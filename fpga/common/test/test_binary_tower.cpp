#include <iostream>
#include "binary_tower.h"

ap_uint<32> dummy_b_mul(ap_uint<32> a, ap_uint<32> b) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
    return binary_tower::b_mul(a, b);
}

ap_uint<32> dummy_b_inv(ap_uint<32> a) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
    return binary_tower::b_inv(a);
}

ap_uint<32> dummy_b_sqr(ap_uint<32> a) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
    return binary_tower::b_sqr(a);
}

int main() {
    ap_uint<32> a = 221;
    ap_uint<32> b = 168;
    ap_uint<32> c = binary_tower::b_mul(a, b);
    ap_uint<32> c_inv = binary_tower::b_inv(c);
    std::cout << c_inv << std::endl;
    std::cout << binary_tower::b_mul(c, c_inv) << std::endl;
    return 0;
}
