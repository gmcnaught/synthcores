// SPDX-License-Identifier: MIT
// synthframe: battery-backed RAM <-> the MiSTer save file (hps_io SD slot 0)
//
// A core whose image is loaded through an "FS" entry in CONF_STR ("FSC" also reloads the last
// image when the core starts) gets saves/<core>/<image>.sav mounted on slot 0 by Main_MiSTer
// at the end of the download, while ioctl_download is still high. This module:
//   - restore: after the download, when the image is good and the mounted file holds at least
//     2**AW bytes, reads it into the RAM through mem_we/mem_addr/mem_wdata; boot_hold keeps
//     the core in reset until then (a missing or short file leaves the RAM as it is);
//   - save: on a rising save_req, or when the OSD opens with autosave set, writes the RAM to the
//     file, reading mem_rdata at mem_addr (the byte one clock after the address) from a copy of
//     the RAM with its own read port.
//     SHADOW = 1 (default): pause goes up until the core reports idle (no step in progress)
//     for 8 clocks, the RAM is copied into a shadow block RAM in 2**AW + 1 clocks, pause comes
//     down, and the shadow goes to the file while the core runs. One pass; the file is the RAM
//     at one instant; the core stops for about 2**AW clocks (164 us for 8 KB at 50 MHz), less
//     than a sound FIFO holds. Costs 2**AW bytes of block RAM (8 KB = 8 M10K).
//     SHADOW = 0: the RAM goes to the file while the core keeps running. If the core writes the
//     RAM (mem_written) during that pass, the file may hold a mix of old and new bytes, so it
//     is written again with pause up for the whole pass, once the core reports idle: the
//     second pass is consistent, but the core stops for the whole SD write (tens of ms);
//   - writes nothing to a read-only file.
// rst (power-on, MiSTer RESET, a user reset) stops a transfer in progress and holds the core
// (boot_hold) until any restore still owed for the last image download is done; it keeps the
// mounted file, so saves keep working after a reset, and it does not read the file again (the
// RAM keeps its contents through a reset, as battery-backed RAM does). Only a new image
// download (loading rising) forgets the mount and restores again. A transfer the HPS has begun
// (sd_ack high) runs to its end after rst; the next request waits for it.
// The file is the RAM's bytes in address order, 2**AW bytes, moved in requests of up to 16 KB
// (hps_io's limit; sd_blk_cnt + 1 blocks of 512 bytes).
module synthframe_nvsave #(
    parameter int AW = 15,                // RAM size 2**AW bytes, 9 .. 20
    parameter bit SHADOW = 1'b1           // 1: copy to a shadow RAM while paused, then write it
) (
    input  logic          clk,
    input  logic          rst,
    input  logic          loading,        // image download in progress (ioctl_download)
    input  logic          image_ok,       // a usable image is loaded

    // hps_io SD slot 0
    input  logic          img_mounted,
    input  logic          img_readonly,
    input  logic [63:0]   img_size,
    output logic [31:0]   sd_lba,
    output logic [5:0]    sd_blk_cnt,
    output logic          sd_rd,
    output logic          sd_wr,
    input  logic          sd_ack,
    input  logic [13:0]   sd_buff_addr,
    input  logic [7:0]    sd_buff_dout,
    input  logic          sd_buff_wr,
    output logic [7:0]    sd_buff_din,

    // triggers
    input  logic          save_req,       // level; the rising edge saves
    input  logic          osd_open,       // OSD_STATUS
    input  logic          autosave,       // save when the OSD opens
    input  logic          mem_written,    // the core wrote the RAM this clock

    // the RAM
    output logic          mem_we,
    output logic [AW-1:0] mem_addr,
    output logic [7:0]    mem_wdata,
    input  logic [7:0]    mem_rdata,      // byte at mem_addr, one clock later

    // the core
    input  logic          core_idle,      // no step in progress
    output logic          boot_hold,      // keep the core in reset (restoring)
    output logic          pause,          // start no steps (saving)
    output logic          saving,
    output logic          restored        // the RAM came from the file at the last load
);
    localparam int CAW = (AW < 14) ? AW : 14;             // bytes per request: 2**CAW
    localparam int NCH = 1 << (AW - CAW);                  // requests per file
    localparam logic [31:0] LBA_STEP = 32'(1 << (CAW - 9));

    assign sd_blk_cnt = 6'((1 << (CAW - 9)) - 1);
    logic [19:0] chunk;                                    // request number
    wire [AW-1:0] sd_addr = AW'({chunk, sd_buff_addr[CAW-1:0]});

    typedef enum logic [2:0] { S_IDLE, S_RD, S_RD_ACK, S_WAIT, S_WR, S_WR_ACK, S_COPY } st_t;
    st_t st;
    logic [AW:0] cp_n;                                     // S_COPY: next RAM address to read

    assign mem_addr  = (SHADOW && st == S_COPY) ? cp_n[AW-1:0] : sd_addr;
    assign mem_wdata = sd_buff_dout;

    // the shadow: written from mem_rdata during S_COPY (the byte read the clock before),
    // read at the SD position one clock after it, as mem_rdata is
    if (SHADOW) begin : g_shadow
        logic [7:0]    shadow [1 << AW];
        logic [7:0]    shadow_q;
        logic          cp_v;
        logic [AW-1:0] cp_a;
        always_ff @(posedge clk) begin
            cp_v <= st == S_COPY && !cp_n[AW];
            cp_a <= cp_n[AW-1:0];
            if (cp_v) shadow[cp_a] <= mem_rdata;
            shadow_q <= shadow[sd_addr];
        end
        assign sd_buff_din = shadow_q;
    end else begin : g_direct
        assign sd_buff_din = mem_rdata;
    end
    assign mem_we = (st == S_RD || st == S_RD_ACK) && sd_ack && sd_buff_wr;

    // power-up values, not reset by rst (see the header)
    /* verilator lint_off PROCASSINIT */
    logic mounted = 1'b0, ro = 1'b0, have = 1'b0;
    logic owed = 1'b0;                     // the restore for the last download is still to do
    logic rest = 1'b0;                     // restored
    /* verilator lint_on PROCASSINIT */
    assign restored = rest;
    logic save_d, osd_d, loading_d, dirty;
    logic [3:0] quiet;

    always_ff @(posedge clk) begin
        save_d <= save_req;
        osd_d <= osd_open;
        loading_d <= loading;
        if (img_mounted) begin
            mounted <= 1'b1;
            ro <= img_readonly;
            have <= img_size >= 64'(1 << AW);
        end
        if (loading && !loading_d) begin              // a new image: a new mount, restore again
            mounted <= 1'b0;
            have <= 1'b0;
            owed <= 1'b1;
            rest <= 1'b0;
        end
        if (rst || (loading && !loading_d)) begin
            boot_hold <= 1'b1;
            st <= S_IDLE;
            sd_rd <= 1'b0;
            sd_wr <= 1'b0;
            pause <= 1'b0;
            saving <= 1'b0;
        end else begin
            if (saving && mem_written) dirty <= 1'b1;
            case (st)
                S_IDLE:
                    // a request starts only with sd_ack low: rst does not stop a transfer the
                    // HPS has already begun, so let it finish first
                    if (sd_ack) begin
                    end else if (boot_hold) begin
                        if (!loading && image_ok) begin
                            if (mounted && have && owed) begin
                                chunk <= '0;
                                sd_lba <= '0;
                                sd_rd <= 1'b1;
                                st <= S_RD;
                            end else begin
                                boot_hold <= 1'b0;
                                owed <= 1'b0;
                            end
                        end
                    end else if (image_ok && !loading && mounted && !ro &&
                                 ((save_req && !save_d) || (autosave && osd_open && !osd_d))) begin
                        saving <= 1'b1;
                        if (SHADOW) begin               // pause, wait for idle, copy (S_WAIT)
                            pause <= 1'b1;
                            quiet <= '0;
                            st <= S_WAIT;
                        end else begin                  // first pass with the core running;
                            dirty <= mem_written;       // dirty = written since it began
                            chunk <= '0;
                            sd_lba <= '0;
                            sd_wr <= 1'b1;
                            st <= S_WR;
                        end
                    end
                // restore
                S_RD: if (sd_ack) begin sd_rd <= 1'b0; st <= S_RD_ACK; end
                S_RD_ACK:
                    if (!sd_ack) begin
                        if (chunk == 20'(NCH - 1)) begin
                            boot_hold <= 1'b0;
                            rest <= 1'b1;
                            owed <= 1'b0;
                            st <= S_IDLE;
                        end else begin
                            chunk <= chunk + 20'd1;
                            sd_lba <= sd_lba + LBA_STEP;
                            sd_rd <= 1'b1;
                            st <= S_RD;
                        end
                    end
                // the core idle for 8 clocks with pause up (no step can have started): then the
                // copy (SHADOW) or the second pass
                S_WAIT:
                    if (!core_idle) quiet <= '0;
                    else if (quiet != 4'd8) quiet <= quiet + 4'd1;
                    else if (SHADOW) begin
                        cp_n <= '0;
                        st <= S_COPY;
                    end else begin
                        chunk <= '0;
                        sd_lba <= '0;
                        sd_wr <= 1'b1;
                        st <= S_WR;
                    end
                // read every RAM address (the last byte lands in the shadow as cp_n reaches
                // 2**AW), then write the shadow with the core running
                S_COPY:
                    if (!cp_n[AW]) cp_n <= cp_n + 1'd1;
                    else begin
                        pause <= 1'b0;
                        chunk <= '0;
                        sd_lba <= '0;
                        sd_wr <= 1'b1;
                        st <= S_WR;
                    end
                S_WR: if (sd_ack) begin sd_wr <= 1'b0; st <= S_WR_ACK; end
                S_WR_ACK:
                    if (!sd_ack) begin
                        if (!SHADOW && chunk == 20'(NCH - 1) && !pause && (dirty || mem_written)) begin
                            pause <= 1'b1;             // written during the pass: again, paused
                            quiet <= '0;
                            st <= S_WAIT;
                        end else if (chunk == 20'(NCH - 1)) begin
                            pause <= 1'b0;
                            saving <= 1'b0;
                            have <= 1'b1;
                            st <= S_IDLE;
                        end else begin
                            chunk <= chunk + 20'd1;
                            sd_lba <= sd_lba + LBA_STEP;
                            sd_wr <= 1'b1;
                            st <= S_WR;
                        end
                    end
                default: st <= S_IDLE;
            endcase
        end
    end
endmodule
