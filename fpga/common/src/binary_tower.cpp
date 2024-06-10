#include "binary_tower.h"

ap_uint<32> binary_tower_32b_inv(ap_uint<32> a) {
    return binary_tower::b_inv(a);
}

ap_uint<32> binary_tower_32b_mul(ap_uint<32> a, ap_uint<32> b) {
    return binary_tower::b_mul(a, b);
}

ap_uint<32> binary_tower_32b_mul_alpha(ap_uint<32> a) {
    return binary_tower::b_mul_alpha(a);
}

ap_uint<32> binary_tower_32b_sqr(ap_uint<32> a) {
    return binary_tower::b_sqr(a);
}
