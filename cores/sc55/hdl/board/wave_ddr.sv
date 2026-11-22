// Wave ROM in MiSTer's DDR3 (DDRAM_*, Avalon-MM, 64-bit words, burst 1).
//
// Read stream for the PCM chip (sc55_pcm rom_*): byte requests taken with
// req && ready, bytes returned in request order on rvalid. A request in the
// same 8-byte word as the request just before it shares that word's DDR3
// read (the four sample bytes of a voice are usually consecutive), so a
// voice costs about two DDR3 reads; reads are pipelined (commands do not
// wait for data). Write port for loading: one byte per write, held until
// accepted (wbusy). Line port: one 64-bit word at BASE + 0x400000 + 8 * laddr
// (ROM2's upper 256 KB on the mk2, for rom2x_cache), ahead of queued PCM reads;
// returned data is routed by a tag kept per issued read.
//
// The wave ROMs live at BASE (bytes; DDR3 from 0x30000000 is free for cores).
module wave_ddr #(
    parameter logic [31:0] BASE = 32'h3000_0000,
    parameter int QBITS = 4
) (
    input  logic        clk,
    input  logic        rst,

    input  logic        wr,
    input  logic [22:0] waddr,
    input  logic [7:0]  wdata,
    output logic        wbusy,

    input  logic        req,
    input  logic [21:0] addr,
    output logic        ready,
    output logic        rvalid,
    output logic [7:0]  rdata,

    input  logic        lreq,             // held until lvalid
    input  logic [14:0] laddr,
    output logic        lvalid,
    output logic [63:0] ldata,

    input  logic        DDRAM_BUSY,
    output logic [7:0]  DDRAM_BURSTCNT,
    output logic [28:0] DDRAM_ADDR,
    input  logic [63:0] DDRAM_DOUT,
    input  logic        DDRAM_DOUT_READY,
    output logic        DDRAM_RD,
    output logic [63:0] DDRAM_DIN,
    output logic [7:0]  DDRAM_BE,
    output logic        DDRAM_WE
);
    localparam int Q = 1 << QBITS;
    localparam logic [28:0] BASE_W = BASE[31:3];

    // request entries {new word read, byte in word}, in order
    logic [3:0]        rq [Q];
    logic [QBITS:0]    rq_wp, rq_rp;
    // DDR3 commands (word offsets) still to issue
    logic [18:0]       cq [Q];
    logic [QBITS:0]    cq_wp, cq_rp;
    // returned words
    logic [63:0]       dq [Q];
    logic [QBITS:0]    dq_wp, dq_rp;
    logic [QBITS:0]    inflight;        // commands issued, data not yet back
    logic              tq [Q];          // per issued read: 1 = line port
    logic [QBITS:0]    tq_wp, tq_rp;
    logic              l_busy;          // the line read is issued or waiting
    logic [18:0]       last_word;
    logic              have_last;
    logic [63:0]       cur;

    wire [18:0] word = addr[21:3];
    wire        same = have_last && word == last_word;
    wire        rq_full = (rq_wp - rq_rp) == (QBITS+1)'(Q);
    wire        cq_full = (cq_wp - cq_rp) == (QBITS+1)'(Q);
    // a new word needs room for its data too (commands issued + queued + words held < Q)
    wire        d_room = ((cq_wp - cq_rp) + inflight + (dq_wp - dq_rp)) < (QBITS+1)'(Q - 1);
    assign ready = !rq_full && !cq_full && d_room && !wbusy;

    assign DDRAM_BURSTCNT = 8'd1;

    always_ff @(posedge clk) begin
        rvalid <= 1'b0;
        lvalid <= 1'b0;
        if (rst) begin
            tq_wp <= '0; tq_rp <= '0; l_busy <= 1'b0;
            rq_wp <= '0; rq_rp <= '0; cq_wp <= '0; cq_rp <= '0; dq_wp <= '0; dq_rp <= '0;
            inflight <= '0;
            have_last <= 1'b0;
            DDRAM_RD <= 1'b0;
            DDRAM_WE <= 1'b0;
            wbusy <= 1'b0;
        end else begin : b_wave_ddr_1
            logic issued, returned;
            issued = 1'b0;
            returned = 1'b0;

            // accept a request
            if (req && ready) begin
                rq[rq_wp[QBITS-1:0]] <= {!same, addr[2:0]};
                rq_wp <= rq_wp + 1'b1;
                if (!same) begin
                    cq[cq_wp[QBITS-1:0]] <= word;
                    cq_wp <= cq_wp + 1'b1;
                end
                last_word <= word;
                have_last <= 1'b1;
            end

            // DDR3 command: a held command goes when the controller is not busy
            if (DDRAM_RD || DDRAM_WE) begin
                if (!DDRAM_BUSY) begin
                    if (DDRAM_RD) issued = 1'b1;
                    DDRAM_RD <= 1'b0;
                    DDRAM_WE <= 1'b0;
                    if (DDRAM_WE) wbusy <= 1'b0;
                end
            end else if (wr && !wbusy) begin
                DDRAM_WE <= 1'b1;
                DDRAM_ADDR <= BASE_W + {9'h0, waddr[22:3]};
                DDRAM_DIN <= {8{wdata}};
                DDRAM_BE <= 8'd1 << waddr[2:0];
                wbusy <= 1'b1;
                have_last <= 1'b0;        // contents change: no merging across a load
            end else if (lreq && !l_busy && !lvalid && inflight < (QBITS+1)'(Q - 1)) begin
                DDRAM_RD <= 1'b1;
                DDRAM_ADDR <= BASE_W + 29'h80000 + {14'h0, laddr};
                tq[tq_wp[QBITS-1:0]] <= 1'b1;
                tq_wp <= tq_wp + 1'b1;
                l_busy <= 1'b1;
            end else if (cq_wp != cq_rp) begin
                DDRAM_RD <= 1'b1;
                DDRAM_ADDR <= BASE_W + {10'h0, cq[cq_rp[QBITS-1:0]]};
                cq_rp <= cq_rp + 1'b1;
                tq[tq_wp[QBITS-1:0]] <= 1'b0;
                tq_wp <= tq_wp + 1'b1;
            end

            // data back, in order
            if (DDRAM_DOUT_READY) begin
                if (tq[tq_rp[QBITS-1:0]]) begin
                    ldata <= DDRAM_DOUT;
                    lvalid <= 1'b1;
                    l_busy <= 1'b0;
                end else begin
                    dq[dq_wp[QBITS-1:0]] <= DDRAM_DOUT;
                    dq_wp <= dq_wp + 1'b1;
                end
                tq_rp <= tq_rp + 1'b1;
                returned = 1'b1;
            end
            inflight <= inflight + (QBITS+1)'(issued) - (QBITS+1)'(returned);

            // bytes out, in request order
            if (rq_wp != rq_rp) begin : b_wave_ddr_2
                logic [3:0] e;
                logic [63:0] w;
                e = rq[rq_rp[QBITS-1:0]];
                if (!e[3] || dq_wp != dq_rp) begin
                    w = cur;
                    if (e[3]) begin
                        w = dq[dq_rp[QBITS-1:0]];
                        dq_rp <= dq_rp + 1'b1;
                        cur <= w;
                    end
                    rdata <= w[8 * e[2:0] +: 8];   // little-endian: byte 0 in bits 7:0
                    rvalid <= 1'b1;
                    rq_rp <= rq_rp + 1'b1;
                end
            end
        end
    end
endmodule
