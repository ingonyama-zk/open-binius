#include <iostream>
#include "binary_tower.h"

ap_uint<32> dummy(ap_uint<32> a, ap_uint<32> b) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
    return binary_tower::b_mul(a, b);
}

int main() {
    ap_uint<8> a = 221;
    ap_uint<8> b = 168;
    std::cout << binary_tower::b_mul(a, b) << std::endl;
    return 0;
}
