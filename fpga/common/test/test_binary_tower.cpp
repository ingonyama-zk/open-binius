#include <iostream>
#include <sstream>
#include <iomanip>
#include "binary_tower.h"

ap_uint<32> dummy(ap_uint<32> a) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INLINE recursive
#pragma HLS PIPELINE II=1
    return binary_tower::b_inv(a);
}

int main() {
    return 0;
}
