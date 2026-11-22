// SPDX-License-Identifier: MIT
// synthframe memory port: N clients onto one port (synthframe_mem_ddr3 or another backend),
// fixed priority (client 0 first). Each client sees the memory-port contract of
// synthframe_mem_ddr3 with a CTW-bit tag; the arbiter adds the client number to the tag on
// the way down and routes each response back by it. Responses stay in request order per
// client because the backend returns all reads in order.
module synthframe_mem_arb #(
    parameter int N = 2,                  // clients
    parameter int CTW = 1,                // client tag width
    // derived; not to be overridden (Quartus 17 takes no localparam in the parameter list)
    parameter int IW = N > 1 ? $clog2(N) : 1,
    parameter int TW = CTW + IW           // tag width towards the backend
) (
    // clients
    input  logic            c_req_valid [N],
    output logic            c_req_ready [N],
    input  logic            c_req_we    [N],
    input  logic [28:0]     c_req_addr  [N],
    input  logic [63:0]     c_req_wdata [N],
    input  logic [7:0]      c_req_be    [N],
    input  logic [CTW-1:0]  c_req_tag   [N],
    output logic            c_rsp_valid [N],
    output logic [63:0]     c_rsp_data,           // shared: valid for client k on c_rsp_valid[k]
    output logic [CTW-1:0]  c_rsp_tag,

    // backend
    output logic            req_valid,
    input  logic            req_ready,
    output logic            req_we,
    output logic [28:0]     req_addr,
    output logic [63:0]     req_wdata,
    output logic [7:0]      req_be,
    output logic [TW-1:0]   req_tag,
    input  logic            rsp_valid,
    input  logic [63:0]     rsp_data,
    input  logic [TW-1:0]   rsp_tag
);
    // grant: the lowest-numbered client with a request (depends on the requests only)
    logic          any;
    logic [IW-1:0] g;
    always_comb begin
        any = 1'b0;
        g = '0;
        for (int k = N - 1; k >= 0; k--)
            if (c_req_valid[k]) begin
                any = 1'b1;
                g = IW'(k);
            end
    end
    assign req_valid = any;
    assign req_we = c_req_we[g];
    assign req_addr = c_req_addr[g];
    assign req_wdata = c_req_wdata[g];
    assign req_be = c_req_be[g];
    assign req_tag = {g, c_req_tag[g]};
    always_comb
        for (int k = 0; k < N; k++) begin
            c_req_ready[k] = any && g == IW'(k) && req_ready;
            c_rsp_valid[k] = rsp_valid && rsp_tag[TW-1:CTW] == IW'(k);
        end
    assign c_rsp_data = rsp_data;
    assign c_rsp_tag = rsp_tag[CTW-1:0];
endmodule
