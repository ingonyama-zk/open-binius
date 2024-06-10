module binary_tower_32b_sqr (
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

wire   [0:0] a0_42_fu_114_p1;
wire   [0:0] a1_36_fu_118_p3;
wire   [0:0] xor_ln59_fu_126_p2;
wire   [0:0] a0_43_fu_140_p3;
wire   [0:0] a1_fu_148_p3;
wire   [0:0] a0_fu_156_p2;
wire   [1:0] a1_sqr_29_fu_162_p3;
wire   [1:0] a0_sqr_1_fu_132_p3;
reg   [1:0] tmp_fu_176_p4;
wire   [1:0] xor_ln59_2_fu_170_p2;
wire   [0:0] a0_44_fu_194_p3;
wire   [0:0] a1_37_fu_202_p3;
wire   [0:0] xor_ln59_3_fu_210_p2;
wire   [0:0] a0_45_fu_224_p3;
wire   [0:0] a1_38_fu_232_p3;
wire   [0:0] a0_5_fu_240_p2;
wire   [1:0] a1_sqr_30_fu_246_p3;
wire   [1:0] a0_sqr_5_fu_216_p3;
reg   [1:0] tmp_16_fu_260_p4;
wire   [1:0] a0_6_fu_254_p2;
wire   [1:0] tmp_6_fu_278_p3;
wire   [3:0] a1_sqr_31_fu_270_p3;
wire   [3:0] a0_sqr_3_fu_186_p3;
wire   [1:0] xor_ln28_fu_286_p2;
wire   [3:0] xor_ln59_6_fu_292_p2;
wire   [0:0] a0_46_fu_308_p3;
wire   [0:0] a1_39_fu_316_p3;
wire   [0:0] xor_ln59_7_fu_324_p2;
wire   [0:0] a0_47_fu_338_p3;
wire   [0:0] a1_7_fu_346_p3;
wire   [0:0] a0_10_fu_354_p2;
wire   [1:0] a1_sqr_32_fu_360_p3;
wire   [1:0] a0_sqr_9_fu_330_p3;
reg   [1:0] tmp_21_fu_374_p4;
wire   [1:0] xor_ln59_9_fu_368_p2;
wire   [0:0] a0_48_fu_392_p3;
wire   [0:0] a1_40_fu_400_p3;
wire   [0:0] xor_ln59_10_fu_408_p2;
wire   [0:0] a0_49_fu_422_p3;
wire   [0:0] a1_41_fu_430_p3;
wire   [0:0] a0_13_fu_438_p2;
wire   [1:0] a1_sqr_33_fu_444_p3;
wire   [1:0] a0_sqr_13_fu_414_p3;
reg   [1:0] a1_42_fu_458_p4;
wire   [1:0] a0_14_fu_452_p2;
wire   [1:0] tmp_2_fu_476_p3;
wire   [3:0] a1_sqr_34_fu_468_p3;
wire   [3:0] a0_sqr_11_fu_384_p3;
wire   [1:0] a1_43_fu_484_p2;
wire   [3:0] a0_16_fu_490_p2;
wire   [0:0] a0_18_fu_506_p1;
wire   [0:0] a1_14_fu_510_p3;
wire   [0:0] xor_ln28_2_fu_518_p2;
wire   [1:0] tmp_5_fu_524_p3;
wire   [1:0] xor_ln28_3_fu_532_p2;
wire   [3:0] tmp_8_fu_538_p3;
wire   [7:0] a1_sqr_35_fu_496_p4;
wire   [7:0] a0_sqr_7_fu_298_p4;
wire   [3:0] xor_ln28_4_fu_546_p2;
wire   [7:0] xor_ln59_14_fu_552_p2;
wire   [0:0] a0_50_fu_570_p3;
wire   [0:0] a1_44_fu_578_p3;
wire   [0:0] xor_ln59_15_fu_586_p2;
wire   [0:0] a0_51_fu_600_p3;
wire   [0:0] a1_17_fu_608_p3;
wire   [0:0] a0_21_fu_616_p2;
wire   [1:0] a1_sqr_36_fu_622_p3;
wire   [1:0] a0_sqr_17_fu_592_p3;
reg   [1:0] tmp_32_fu_636_p4;
wire   [1:0] xor_ln59_17_fu_630_p2;
wire   [0:0] a0_52_fu_654_p3;
wire   [0:0] a1_45_fu_662_p3;
wire   [0:0] xor_ln59_18_fu_670_p2;
wire   [0:0] a0_53_fu_684_p3;
wire   [0:0] a1_46_fu_692_p3;
wire   [0:0] a0_24_fu_700_p2;
wire   [1:0] a1_sqr_37_fu_706_p3;
wire   [1:0] a0_sqr_21_fu_676_p3;
reg   [1:0] tmp_37_fu_720_p4;
wire   [1:0] a0_25_fu_714_p2;
wire   [1:0] tmp_s_fu_738_p3;
wire   [3:0] a1_sqr_38_fu_730_p3;
wire   [3:0] a0_sqr_19_fu_646_p3;
wire   [1:0] xor_ln28_5_fu_746_p2;
wire   [3:0] xor_ln59_21_fu_752_p2;
wire   [0:0] a0_54_fu_768_p3;
wire   [0:0] a1_47_fu_776_p3;
wire   [0:0] xor_ln59_22_fu_784_p2;
wire   [0:0] a0_55_fu_798_p3;
wire   [0:0] a1_24_fu_806_p3;
wire   [0:0] a0_29_fu_814_p2;
wire   [1:0] a1_sqr_fu_820_p3;
wire   [1:0] a0_sqr_25_fu_790_p3;
reg   [1:0] tmp_42_fu_834_p4;
wire   [1:0] xor_ln59_24_fu_828_p2;
wire   [0:0] a0_56_fu_852_p3;
wire   [0:0] a1_48_fu_860_p3;
wire   [0:0] xor_ln59_25_fu_868_p2;
wire   [0:0] a0_57_fu_882_p3;
wire   [0:0] a1_49_fu_890_p3;
wire   [0:0] a0_32_fu_898_p2;
wire   [1:0] a1_sqr_39_fu_904_p3;
wire   [1:0] a0_sqr_29_fu_874_p3;
reg   [1:0] a1_50_fu_918_p4;
wire   [1:0] a0_33_fu_912_p2;
wire   [1:0] tmp_1_fu_936_p3;
wire   [3:0] a1_sqr_40_fu_928_p3;
wire   [3:0] a0_sqr_27_fu_844_p3;
wire   [1:0] a1_52_fu_944_p2;
wire   [3:0] a0_35_fu_950_p2;
wire   [0:0] a0_37_fu_974_p1;
wire   [0:0] a1_32_fu_978_p3;
wire   [0:0] xor_ln28_7_fu_986_p2;
wire   [1:0] tmp_3_fu_992_p3;
wire   [1:0] xor_ln28_8_fu_1000_p2;
wire   [3:0] tmp_4_fu_1006_p3;
wire   [7:0] a1_sqr_41_fu_956_p4;
wire   [7:0] a0_sqr_23_fu_758_p4;
wire   [3:0] a1_53_fu_1014_p2;
wire   [7:0] a0_38_fu_1020_p2;
wire   [0:0] a0_41_fu_1052_p3;
wire   [0:0] a1_35_fu_1060_p3;
wire   [0:0] xor_ln28_10_fu_1068_p2;
wire   [1:0] a0_40_fu_1038_p1;
wire   [1:0] tmp_7_fu_1074_p3;
wire   [1:0] xor_ln28_11_fu_1082_p2;
wire   [1:0] a1_54_fu_1042_p4;
wire   [3:0] a1_51_fu_966_p3;
wire   [3:0] tmp_9_fu_1088_p3;
wire   [3:0] xor_ln28_12_fu_1096_p2;
wire   [7:0] tmp_10_fu_1102_p3;
wire   [15:0] a1_sqr_42_fu_1026_p5;
wire   [15:0] a0_sqr_15_fu_558_p5;
wire   [7:0] xor_ln28_13_fu_1110_p2;
wire   [15:0] xor_ln59_30_fu_1116_p2;

assign a0_10_fu_354_p2 = (a1_7_fu_346_p3 ^ a0_47_fu_338_p3);

assign a0_13_fu_438_p2 = (a1_41_fu_430_p3 ^ a0_49_fu_422_p3);

assign a0_14_fu_452_p2 = (a1_sqr_33_fu_444_p3 ^ a0_sqr_13_fu_414_p3);

assign a0_16_fu_490_p2 = (a1_sqr_34_fu_468_p3 ^ a0_sqr_11_fu_384_p3);

assign a0_18_fu_506_p1 = a1_43_fu_484_p2[0:0];

assign a0_21_fu_616_p2 = (a1_17_fu_608_p3 ^ a0_51_fu_600_p3);

assign a0_24_fu_700_p2 = (a1_46_fu_692_p3 ^ a0_53_fu_684_p3);

assign a0_25_fu_714_p2 = (a1_sqr_37_fu_706_p3 ^ a0_sqr_21_fu_676_p3);

assign a0_29_fu_814_p2 = (a1_24_fu_806_p3 ^ a0_55_fu_798_p3);

assign a0_32_fu_898_p2 = (a1_49_fu_890_p3 ^ a0_57_fu_882_p3);

assign a0_33_fu_912_p2 = (a1_sqr_39_fu_904_p3 ^ a0_sqr_29_fu_874_p3);

assign a0_35_fu_950_p2 = (a1_sqr_40_fu_928_p3 ^ a0_sqr_27_fu_844_p3);

assign a0_37_fu_974_p1 = a1_52_fu_944_p2[0:0];

assign a0_38_fu_1020_p2 = (a1_sqr_41_fu_956_p4 ^ a0_sqr_23_fu_758_p4);

assign a0_40_fu_1038_p1 = a1_53_fu_1014_p2[1:0];

assign a0_41_fu_1052_p3 = a1_53_fu_1014_p2[32'd2];

assign a0_42_fu_114_p1 = a[0:0];

assign a0_43_fu_140_p3 = a[32'd2];

assign a0_44_fu_194_p3 = a[32'd4];

assign a0_45_fu_224_p3 = a[32'd6];

assign a0_46_fu_308_p3 = a[32'd8];

assign a0_47_fu_338_p3 = a[32'd10];

assign a0_48_fu_392_p3 = a[32'd12];

assign a0_49_fu_422_p3 = a[32'd14];

assign a0_50_fu_570_p3 = a[32'd16];

assign a0_51_fu_600_p3 = a[32'd18];

assign a0_52_fu_654_p3 = a[32'd20];

assign a0_53_fu_684_p3 = a[32'd22];

assign a0_54_fu_768_p3 = a[32'd24];

assign a0_55_fu_798_p3 = a[32'd26];

assign a0_56_fu_852_p3 = a[32'd28];

assign a0_57_fu_882_p3 = a[32'd30];

assign a0_5_fu_240_p2 = (a1_38_fu_232_p3 ^ a0_45_fu_224_p3);

assign a0_6_fu_254_p2 = (a1_sqr_30_fu_246_p3 ^ a0_sqr_5_fu_216_p3);

assign a0_fu_156_p2 = (a1_fu_148_p3 ^ a0_43_fu_140_p3);

assign a0_sqr_11_fu_384_p3 = {{tmp_21_fu_374_p4}, {xor_ln59_9_fu_368_p2}};

assign a0_sqr_13_fu_414_p3 = {{a1_40_fu_400_p3}, {xor_ln59_10_fu_408_p2}};

assign a0_sqr_15_fu_558_p5 = {{{{xor_ln28_4_fu_546_p2}, {a1_43_fu_484_p2}}, {a1_42_fu_458_p4}}, {xor_ln59_14_fu_552_p2}};

assign a0_sqr_17_fu_592_p3 = {{a1_44_fu_578_p3}, {xor_ln59_15_fu_586_p2}};

assign a0_sqr_19_fu_646_p3 = {{tmp_32_fu_636_p4}, {xor_ln59_17_fu_630_p2}};

assign a0_sqr_1_fu_132_p3 = {{a1_36_fu_118_p3}, {xor_ln59_fu_126_p2}};

assign a0_sqr_21_fu_676_p3 = {{a1_45_fu_662_p3}, {xor_ln59_18_fu_670_p2}};

assign a0_sqr_23_fu_758_p4 = {{{xor_ln28_5_fu_746_p2}, {tmp_37_fu_720_p4}}, {xor_ln59_21_fu_752_p2}};

assign a0_sqr_25_fu_790_p3 = {{a1_47_fu_776_p3}, {xor_ln59_22_fu_784_p2}};

assign a0_sqr_27_fu_844_p3 = {{tmp_42_fu_834_p4}, {xor_ln59_24_fu_828_p2}};

assign a0_sqr_29_fu_874_p3 = {{a1_48_fu_860_p3}, {xor_ln59_25_fu_868_p2}};

assign a0_sqr_3_fu_186_p3 = {{tmp_fu_176_p4}, {xor_ln59_2_fu_170_p2}};

assign a0_sqr_5_fu_216_p3 = {{a1_37_fu_202_p3}, {xor_ln59_3_fu_210_p2}};

assign a0_sqr_7_fu_298_p4 = {{{xor_ln28_fu_286_p2}, {tmp_16_fu_260_p4}}, {xor_ln59_6_fu_292_p2}};

assign a0_sqr_9_fu_330_p3 = {{a1_39_fu_316_p3}, {xor_ln59_7_fu_324_p2}};

assign a1_14_fu_510_p3 = a1_43_fu_484_p2[32'd1];

assign a1_17_fu_608_p3 = a[32'd19];

assign a1_24_fu_806_p3 = a[32'd27];

assign a1_32_fu_978_p3 = a1_52_fu_944_p2[32'd1];

assign a1_35_fu_1060_p3 = a1_53_fu_1014_p2[32'd3];

assign a1_36_fu_118_p3 = a[32'd1];

assign a1_37_fu_202_p3 = a[32'd5];

assign a1_38_fu_232_p3 = a[32'd7];

assign a1_39_fu_316_p3 = a[32'd9];

assign a1_40_fu_400_p3 = a[32'd13];

assign a1_41_fu_430_p3 = a[32'd15];

integer ap_tvar_int_0;

always @ (a) begin
    for (ap_tvar_int_0 = 2 - 1; ap_tvar_int_0 >= 0; ap_tvar_int_0 = ap_tvar_int_0 - 1) begin
        if (ap_tvar_int_0 > 15 - 14) begin
            a1_42_fu_458_p4[ap_tvar_int_0] = 1'b0;
        end else begin
            a1_42_fu_458_p4[ap_tvar_int_0] = a[15 - ap_tvar_int_0];
        end
    end
end

assign a1_43_fu_484_p2 = (tmp_2_fu_476_p3 ^ a0_14_fu_452_p2);

assign a1_44_fu_578_p3 = a[32'd17];

assign a1_45_fu_662_p3 = a[32'd21];

assign a1_46_fu_692_p3 = a[32'd23];

assign a1_47_fu_776_p3 = a[32'd25];

assign a1_48_fu_860_p3 = a[32'd29];

assign a1_49_fu_890_p3 = a[32'd31];

integer ap_tvar_int_1;

always @ (a) begin
    for (ap_tvar_int_1 = 2 - 1; ap_tvar_int_1 >= 0; ap_tvar_int_1 = ap_tvar_int_1 - 1) begin
        if (ap_tvar_int_1 > 31 - 30) begin
            a1_50_fu_918_p4[ap_tvar_int_1] = 1'b0;
        end else begin
            a1_50_fu_918_p4[ap_tvar_int_1] = a[31 - ap_tvar_int_1];
        end
    end
end

assign a1_51_fu_966_p3 = {{a1_52_fu_944_p2}, {a1_50_fu_918_p4}};

assign a1_52_fu_944_p2 = (tmp_1_fu_936_p3 ^ a0_33_fu_912_p2);

assign a1_53_fu_1014_p2 = (tmp_4_fu_1006_p3 ^ a0_35_fu_950_p2);

assign a1_54_fu_1042_p4 = {{a1_53_fu_1014_p2[3:2]}};

assign a1_7_fu_346_p3 = a[32'd11];

assign a1_fu_148_p3 = a[32'd3];

assign a1_sqr_29_fu_162_p3 = {{a1_fu_148_p3}, {a0_fu_156_p2}};

assign a1_sqr_30_fu_246_p3 = {{a1_38_fu_232_p3}, {a0_5_fu_240_p2}};

assign a1_sqr_31_fu_270_p3 = {{tmp_16_fu_260_p4}, {a0_6_fu_254_p2}};

assign a1_sqr_32_fu_360_p3 = {{a1_7_fu_346_p3}, {a0_10_fu_354_p2}};

assign a1_sqr_33_fu_444_p3 = {{a1_41_fu_430_p3}, {a0_13_fu_438_p2}};

assign a1_sqr_34_fu_468_p3 = {{a1_42_fu_458_p4}, {a0_14_fu_452_p2}};

assign a1_sqr_35_fu_496_p4 = {{{a1_43_fu_484_p2}, {a1_42_fu_458_p4}}, {a0_16_fu_490_p2}};

assign a1_sqr_36_fu_622_p3 = {{a1_17_fu_608_p3}, {a0_21_fu_616_p2}};

assign a1_sqr_37_fu_706_p3 = {{a1_46_fu_692_p3}, {a0_24_fu_700_p2}};

assign a1_sqr_38_fu_730_p3 = {{tmp_37_fu_720_p4}, {a0_25_fu_714_p2}};

assign a1_sqr_39_fu_904_p3 = {{a1_49_fu_890_p3}, {a0_32_fu_898_p2}};

assign a1_sqr_40_fu_928_p3 = {{a1_50_fu_918_p4}, {a0_33_fu_912_p2}};

assign a1_sqr_41_fu_956_p4 = {{{a1_52_fu_944_p2}, {a1_50_fu_918_p4}}, {a0_35_fu_950_p2}};

assign a1_sqr_42_fu_1026_p5 = {{{{a1_53_fu_1014_p2}, {a1_52_fu_944_p2}}, {a1_50_fu_918_p4}}, {a0_38_fu_1020_p2}};

assign a1_sqr_fu_820_p3 = {{a1_24_fu_806_p3}, {a0_29_fu_814_p2}};

assign tmp_10_fu_1102_p3 = {{xor_ln28_12_fu_1096_p2}, {a1_53_fu_1014_p2}};

integer ap_tvar_int_2;

always @ (a) begin
    for (ap_tvar_int_2 = 2 - 1; ap_tvar_int_2 >= 0; ap_tvar_int_2 = ap_tvar_int_2 - 1) begin
        if (ap_tvar_int_2 > 7 - 6) begin
            tmp_16_fu_260_p4[ap_tvar_int_2] = 1'b0;
        end else begin
            tmp_16_fu_260_p4[ap_tvar_int_2] = a[7 - ap_tvar_int_2];
        end
    end
end

assign tmp_1_fu_936_p3 = {{a0_32_fu_898_p2}, {a0_57_fu_882_p3}};

integer ap_tvar_int_3;

always @ (a) begin
    for (ap_tvar_int_3 = 2 - 1; ap_tvar_int_3 >= 0; ap_tvar_int_3 = ap_tvar_int_3 - 1) begin
        if (ap_tvar_int_3 > 11 - 10) begin
            tmp_21_fu_374_p4[ap_tvar_int_3] = 1'b0;
        end else begin
            tmp_21_fu_374_p4[ap_tvar_int_3] = a[11 - ap_tvar_int_3];
        end
    end
end

assign tmp_2_fu_476_p3 = {{a0_13_fu_438_p2}, {a0_49_fu_422_p3}};

integer ap_tvar_int_4;

always @ (a) begin
    for (ap_tvar_int_4 = 2 - 1; ap_tvar_int_4 >= 0; ap_tvar_int_4 = ap_tvar_int_4 - 1) begin
        if (ap_tvar_int_4 > 19 - 18) begin
            tmp_32_fu_636_p4[ap_tvar_int_4] = 1'b0;
        end else begin
            tmp_32_fu_636_p4[ap_tvar_int_4] = a[19 - ap_tvar_int_4];
        end
    end
end

integer ap_tvar_int_5;

always @ (a) begin
    for (ap_tvar_int_5 = 2 - 1; ap_tvar_int_5 >= 0; ap_tvar_int_5 = ap_tvar_int_5 - 1) begin
        if (ap_tvar_int_5 > 23 - 22) begin
            tmp_37_fu_720_p4[ap_tvar_int_5] = 1'b0;
        end else begin
            tmp_37_fu_720_p4[ap_tvar_int_5] = a[23 - ap_tvar_int_5];
        end
    end
end

assign tmp_3_fu_992_p3 = {{xor_ln28_7_fu_986_p2}, {a1_32_fu_978_p3}};

integer ap_tvar_int_6;

always @ (a) begin
    for (ap_tvar_int_6 = 2 - 1; ap_tvar_int_6 >= 0; ap_tvar_int_6 = ap_tvar_int_6 - 1) begin
        if (ap_tvar_int_6 > 27 - 26) begin
            tmp_42_fu_834_p4[ap_tvar_int_6] = 1'b0;
        end else begin
            tmp_42_fu_834_p4[ap_tvar_int_6] = a[27 - ap_tvar_int_6];
        end
    end
end

assign tmp_4_fu_1006_p3 = {{xor_ln28_8_fu_1000_p2}, {a1_52_fu_944_p2}};

assign tmp_5_fu_524_p3 = {{xor_ln28_2_fu_518_p2}, {a1_14_fu_510_p3}};

assign tmp_6_fu_278_p3 = {{a0_5_fu_240_p2}, {a0_45_fu_224_p3}};

assign tmp_7_fu_1074_p3 = {{xor_ln28_10_fu_1068_p2}, {a1_35_fu_1060_p3}};

assign tmp_8_fu_538_p3 = {{xor_ln28_3_fu_532_p2}, {a1_43_fu_484_p2}};

assign tmp_9_fu_1088_p3 = {{xor_ln28_11_fu_1082_p2}, {a1_54_fu_1042_p4}};

integer ap_tvar_int_7;

always @ (a) begin
    for (ap_tvar_int_7 = 2 - 1; ap_tvar_int_7 >= 0; ap_tvar_int_7 = ap_tvar_int_7 - 1) begin
        if (ap_tvar_int_7 > 3 - 2) begin
            tmp_fu_176_p4[ap_tvar_int_7] = 1'b0;
        end else begin
            tmp_fu_176_p4[ap_tvar_int_7] = a[3 - ap_tvar_int_7];
        end
    end
end

assign tmp_s_fu_738_p3 = {{a0_24_fu_700_p2}, {a0_53_fu_684_p3}};

assign xor_ln28_10_fu_1068_p2 = (a1_35_fu_1060_p3 ^ a0_41_fu_1052_p3);

assign xor_ln28_11_fu_1082_p2 = (tmp_7_fu_1074_p3 ^ a0_40_fu_1038_p1);

assign xor_ln28_12_fu_1096_p2 = (tmp_9_fu_1088_p3 ^ a1_51_fu_966_p3);

assign xor_ln28_13_fu_1110_p2 = (tmp_10_fu_1102_p3 ^ a0_38_fu_1020_p2);

assign xor_ln28_2_fu_518_p2 = (a1_14_fu_510_p3 ^ a0_18_fu_506_p1);

assign xor_ln28_3_fu_532_p2 = (tmp_5_fu_524_p3 ^ a1_42_fu_458_p4);

assign xor_ln28_4_fu_546_p2 = (tmp_8_fu_538_p3 ^ a0_16_fu_490_p2);

assign xor_ln28_5_fu_746_p2 = (tmp_s_fu_738_p3 ^ a0_25_fu_714_p2);

assign xor_ln28_7_fu_986_p2 = (a1_32_fu_978_p3 ^ a0_37_fu_974_p1);

assign xor_ln28_8_fu_1000_p2 = (tmp_3_fu_992_p3 ^ a1_50_fu_918_p4);

assign xor_ln28_fu_286_p2 = (tmp_6_fu_278_p3 ^ a0_6_fu_254_p2);

assign xor_ln59_10_fu_408_p2 = (a1_40_fu_400_p3 ^ a0_48_fu_392_p3);

assign xor_ln59_14_fu_552_p2 = (a1_sqr_35_fu_496_p4 ^ a0_sqr_7_fu_298_p4);

assign xor_ln59_15_fu_586_p2 = (a1_44_fu_578_p3 ^ a0_50_fu_570_p3);

assign xor_ln59_17_fu_630_p2 = (a1_sqr_36_fu_622_p3 ^ a0_sqr_17_fu_592_p3);

assign xor_ln59_18_fu_670_p2 = (a1_45_fu_662_p3 ^ a0_52_fu_654_p3);

assign xor_ln59_21_fu_752_p2 = (a1_sqr_38_fu_730_p3 ^ a0_sqr_19_fu_646_p3);

assign xor_ln59_22_fu_784_p2 = (a1_47_fu_776_p3 ^ a0_54_fu_768_p3);

assign xor_ln59_24_fu_828_p2 = (a1_sqr_fu_820_p3 ^ a0_sqr_25_fu_790_p3);

assign xor_ln59_25_fu_868_p2 = (a1_48_fu_860_p3 ^ a0_56_fu_852_p3);

assign xor_ln59_2_fu_170_p2 = (a1_sqr_29_fu_162_p3 ^ a0_sqr_1_fu_132_p3);

assign xor_ln59_30_fu_1116_p2 = (a1_sqr_42_fu_1026_p5 ^ a0_sqr_15_fu_558_p5);

assign xor_ln59_3_fu_210_p2 = (a1_37_fu_202_p3 ^ a0_44_fu_194_p3);

assign xor_ln59_6_fu_292_p2 = (a1_sqr_31_fu_270_p3 ^ a0_sqr_3_fu_186_p3);

assign xor_ln59_7_fu_324_p2 = (a1_39_fu_316_p3 ^ a0_46_fu_308_p3);

assign xor_ln59_9_fu_368_p2 = (a1_sqr_32_fu_360_p3 ^ a0_sqr_9_fu_330_p3);

assign xor_ln59_fu_126_p2 = (a1_36_fu_118_p3 ^ a0_42_fu_114_p1);

assign ap_return = {{{{{xor_ln28_13_fu_1110_p2}, {a1_53_fu_1014_p2}}, {a1_52_fu_944_p2}}, {a1_50_fu_918_p4}}, {xor_ln59_30_fu_1116_p2}};

endmodule //binary_tower_32b_sqr