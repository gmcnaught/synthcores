// Shared by the panel benches: a xorshift32 step for seeded panel contents.
function automatic logic [31:0] xs(input logic [31:0] v);
    logic [31:0] s;
    s = v;
    s = s ^ (s << 13);
    s = s ^ (s >> 17);
    s = s ^ (s << 5);
    return s;
endfunction
