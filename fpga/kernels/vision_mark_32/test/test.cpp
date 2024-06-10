#include "binary_tower.h"
#include "vision_mark_32.h"

int main() {
    for (int a = 0; a < 16; a++) {
        std::cout << ap_uint<4>(binary_tower::b_mul_alpha<4>(a)).to_string(16) << ",";
    }
//    for (int a = 0; a < 256; a++) {
//        std::cout << ap_uint<8>(binary_tower::b_sqr<32>(a)).to_string(16) << ",";
//    }
//
//    std::cout << std::endl;
//    for (int a = 0; a < 16; a++) {
//        std::cout << std::hex << binary_tower_32_t(a).sqr().raw << ",";
//    }
    return 0;
}