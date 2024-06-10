#include <iostream>
#include <sstream>
#include <iomanip>
#include "binary_tower.h"

ap_uint<32> dummy(ap_uint<32> x) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INLINE recursive
#pragma HLS PIPELINE II=1
    return binary_tower_t<32>(x).mul_alpha().raw;
}

int main() {
    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << binary_tower::b_mul_alpha<4>(i) << ", ";
    }
    return 0;
}
