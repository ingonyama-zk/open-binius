module binary_tower_32b_mul_alpha (
        ap_clk,
        ap_ce,
        a,
        ap_return,
        ap_rst
);

input ap_clk;
input ap_ce;
input  [31:0] a;
output  [31:0] ap_return;
input   ap_rst;

wire   [0:0] a0_4_fu_142_p3;
wire   [0:0] a1_fu_150_p3;
wire   [0:0] xor_ln26_fu_158_p2;
wire   [1:0] tmp_fu_164_p3;
wire   [1:0] a0_3_fu_122_p4;
wire   [1:0] xor_ln26_1_fu_172_p2;
wire   [1:0] a1_4_fu_132_p4;
wire   [3:0] tmp_1_fu_178_p3;
wire   [3:0] a0_2_fu_102_p4;
wire   [3:0] xor_ln26_2_fu_186_p2;
wire   [3:0] a1_3_fu_112_p4;
wire   [7:0] tmp_2_fu_192_p3;
wire   [7:0] a0_1_fu_82_p4;
wire   [7:0] xor_ln26_3_fu_200_p2;
wire   [7:0] a1_2_fu_92_p4;
wire   [15:0] tmp_3_fu_206_p3;
wire   [15:0] a0_fu_68_p1;
wire   [15:0] xor_ln26_4_fu_214_p2;
wire   [15:0] a1_1_fu_72_p4;

assign a0_1_fu_82_p4 = {{a[23:16]}};

assign a0_2_fu_102_p4 = {{a[27:24]}};

assign a0_3_fu_122_p4 = {{a[29:28]}};

assign a0_4_fu_142_p3 = a[32'd30];

assign a0_fu_68_p1 = a[15:0];

assign a1_1_fu_72_p4 = {{a[31:16]}};

assign a1_2_fu_92_p4 = {{a[31:24]}};

assign a1_3_fu_112_p4 = {{a[31:28]}};

assign a1_4_fu_132_p4 = {{a[31:30]}};

assign a1_fu_150_p3 = a[32'd31];

assign tmp_1_fu_178_p3 = {{xor_ln26_1_fu_172_p2}, {a1_4_fu_132_p4}};

assign tmp_2_fu_192_p3 = {{xor_ln26_2_fu_186_p2}, {a1_3_fu_112_p4}};

assign tmp_3_fu_206_p3 = {{xor_ln26_3_fu_200_p2}, {a1_2_fu_92_p4}};

assign tmp_fu_164_p3 = {{xor_ln26_fu_158_p2}, {a1_fu_150_p3}};

assign xor_ln26_1_fu_172_p2 = (tmp_fu_164_p3 ^ a0_3_fu_122_p4);

assign xor_ln26_2_fu_186_p2 = (tmp_1_fu_178_p3 ^ a0_2_fu_102_p4);

assign xor_ln26_3_fu_200_p2 = (tmp_2_fu_192_p3 ^ a0_1_fu_82_p4);

assign xor_ln26_4_fu_214_p2 = (tmp_3_fu_206_p3 ^ a0_fu_68_p1);

assign xor_ln26_fu_158_p2 = (a1_fu_150_p3 ^ a0_4_fu_142_p3);

assign ap_return = {{xor_ln26_4_fu_214_p2}, {a1_1_fu_72_p4}};

endmodule //binary_tower_32b_mul_alpha