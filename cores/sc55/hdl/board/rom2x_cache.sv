// The mk2's ROM2 upper 256 KB (DDR3, wave_ddr line port) behind a 2 KB
// direct-mapped cache of 8-byte lines. The firmware reads it rarely (none in
// 30 M steps of loud MIDI: sim/tools/rom2_stats.cpp); a miss costs one DDR3
// read. Byte reads: req held until ack; addr is the offset in the upper half.
// The tags are cleared while `flush` is high (reset, pack download).
module rom2x_cache (
    input  logic        clk,
    input  logic        flush,
    input  logic        req,
    input  logic [17:0] addr,
    output logic        ack,
    output logic [7:0]  data,

    output logic        lreq,
    output logic [14:0] laddr,
    input  logic        lvalid,
    input  logic [63:0] ldata
);
    logic [255:0] valid;
    logic         lwe;
    logic [63:0]  line_q;
    logic [6:0]   tag_q;
    sdp_ram #(.AW(8), .DW(64)) u_data (
        .clk, .we(lwe), .wa(addr[10:3]), .wd(ldata), .ra(addr[10:3]), .rd(line_q));
    sdp_ram #(.AW(8), .DW(7)) u_tags (
        .clk, .we(lwe), .wa(addr[10:3]), .wd(addr[17:11]), .ra(addr[10:3]), .rd(tag_q));

    typedef enum logic [1:0] { C_IDLE, C_LOOK, C_FILL } c_t;
    c_t st;
    wire hit = valid[addr[10:3]] && tag_q == addr[17:11];

    always_ff @(posedge clk) begin
        ack <= 1'b0;
        lwe <= 1'b0;
        if (flush) begin
            valid <= '0;
            st <= C_IDLE;
            lreq <= 1'b0;
        end else begin
            case (st)
                C_IDLE: if (req && !ack) st <= C_LOOK;      // the line's data and tag are read this clock
                C_LOOK: begin
                    if (hit) begin
                        data <= line_q[8 * addr[2:0] +: 8];
                        ack <= 1'b1;
                        st <= C_IDLE;
                    end else begin
                        lreq <= 1'b1;
                        laddr <= addr[17:3];
                        st <= C_FILL;
                    end
                end
                default: begin                              // C_FILL
                    if (lvalid) begin
                        lreq <= 1'b0;
                        lwe <= 1'b1;
                        valid[addr[10:3]] <= 1'b1;
                        data <= ldata[8 * addr[2:0] +: 8];
                        ack <= 1'b1;
                        st <= C_IDLE;
                    end
                end
            endcase
        end
    end
endmodule
