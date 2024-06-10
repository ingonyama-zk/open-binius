#pragma once

#include <ap_int.h>
#include <string>

ap_uint<32> binary_tower_32b_inv(ap_uint<32> a);

ap_uint<32> binary_tower_32b_mul(ap_uint<32> a, ap_uint<32> b);

ap_uint<32> binary_tower_32b_mul_alpha(ap_uint<32> a);

ap_uint<32> binary_tower_32b_sqr(ap_uint<32> a);

namespace binary_tower {
    template<int W>
    ap_uint<W> b_add(ap_uint<W> a, ap_uint<W> b) {
#pragma HLS INLINE
        return a ^ b;
    }

    template<int W>
    ap_uint<W> b_sub(ap_uint<W> a, ap_uint<W> b) {
#pragma HLS INLINE
        return a ^ b;
    }

    template<int W>
    ap_uint<W> b_mul_alpha(ap_uint<W> a) {
        if constexpr (W == 1) {
            return a;
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            return (a0 ^ b_mul_alpha(a1), a1);
        }
    }

    template<int W>
    ap_uint<W> b_mul(ap_uint<W> a, ap_uint<W> b) {
#pragma HLS INLINE
        if constexpr (W == 1) {
            return a & b;
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            ap_uint<W / 2> b0 = b;
            ap_uint<W / 2> b1 = b >> W / 2;
            ap_uint<W / 2> z0 = b_mul(a0, b0);
            ap_uint<W / 2> z1 = b_mul(a0 ^ a1, b0 ^ b1);
            ap_uint<W / 2> z2 = b_mul(a1, b1);
            return ((z1 ^ z0 ^ z2) ^ b_mul_alpha(z2), z0 ^ z2);
        }
    }

    template<int W>
    ap_uint<W> b_sqr(ap_uint<W> a) {
#pragma HLS INLINE
        if constexpr (W == 1) {
            return a;
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            ap_uint<W / 2> a0_sqr = b_sqr(a0);
            ap_uint<W / 2> a1_sqr = b_sqr(a1);
            return (b_mul_alpha(a1_sqr), a0_sqr ^ a1_sqr);
        }
    }

    template<int W>
    ap_uint<W> b_inv(ap_uint<W> a) {
        if constexpr (W == 1) {
            return a;
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            ap_uint<W / 2> a0z1 = a0 ^ b_mul_alpha(a1);
            ap_uint<W / 2> delta = b_mul(a0, a0z1) ^ b_sqr(a1);
            ap_uint<W / 2> delta_inv = b_inv(delta);
            return (b_mul(delta_inv, a1), b_mul(delta_inv, a0z1));
        }
    }
}

template<int W>
struct binary_tower_t {
    ap_uint<W> raw;

    binary_tower_t() = default;

    explicit binary_tower_t(ap_uint<W> x) : raw(x) {}

    binary_tower_t operator+(const binary_tower_t &other) const {
        return binary_tower_t(raw ^ other.raw);
    }

    binary_tower_t operator-(const binary_tower_t &other) const {
        return binary_tower_t(raw ^ other.raw);
    }

    binary_tower_t &operator+=(const binary_tower_t &other) {
        raw ^= other.raw;
        return *this;
    }

    binary_tower_t &operator-=(const binary_tower_t &other) {
        raw ^= other.raw;
        return *this;
    }

    binary_tower_t operator*(const binary_tower_t &other) const {
        if (W == 32) {
            return binary_tower_t(binary_tower_32b_mul(raw, other.raw));
        } else {
            return binary_tower_t(binary_tower::b_mul(raw, other.raw));
        }
    }

    binary_tower_t mul_alpha() const {
        if (W == 32) {
            return binary_tower_t(binary_tower_32b_mul_alpha(raw));
        } else {
            return binary_tower_t(binary_tower::b_mul_alpha(raw));
        }
    }

    binary_tower_t sqr() const {
        if (W == 32) {
            return binary_tower_t(binary_tower_32b_sqr(raw));
        } else {
            return binary_tower_t(binary_tower::b_sqr(raw));
        }
    }

    binary_tower_t inv() const {
        if (W == 32) {
            return binary_tower_t(binary_tower_32b_inv(raw));
        } else {
            return binary_tower_t(binary_tower::b_inv(raw));
        }
    }
};

//struct binary_tower_8_t {
//    uint8_t raw;
//
//    binary_tower_8_t() = default;
//
//    explicit binary_tower_8_t(uint8_t x) : raw(x) {}
//
//    binary_tower_8_t operator+(const binary_tower_8_t &other) const {
//        return binary_tower_8_t(raw ^ other.raw);
//    }
//
//    binary_tower_8_t operator-(const binary_tower_8_t &other) const {
//        return binary_tower_8_t(raw ^ other.raw);
//    }
//
//    binary_tower_8_t &operator+=(const binary_tower_8_t &other) {
//        raw ^= other.raw;
//        return *this;
//    }
//
//    binary_tower_8_t &operator-=(const binary_tower_8_t &other) {
//        raw ^= other.raw;
//        return *this;
//    }
//
//    binary_tower_8_t operator*(const binary_tower_8_t &other) const {
//        constexpr uint8_t mul_lut[256] = {
//
//        };
//        constexpr uint8_t mul_alpha_lut[16] = {
//                0x0, 0x4, 0x8, 0xc, 0x9, 0xd, 0x1, 0x5, 0xe, 0xa, 0x6, 0x2, 0x7, 0x3, 0xf, 0xb
//        };
//
//
//        return binary_tower_t(binary_tower::b_mul(raw, other.raw));
//    }
////
////    binary_tower_8_t mul_alpha() const {
////        return binary_tower_t(binary_tower::b_mul_alpha(raw));
////    }
////
////    binary_tower_8_t sqr() const {
////        return binary_tower_t(binary_tower::b_sqr(raw));
////    }
////
////    binary_tower_8_t inv() const {
////        return binary_tower_t(binary_tower::b_inv(raw));
////    }
//};