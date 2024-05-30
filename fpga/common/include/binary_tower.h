#pragma once

#include <string>
#include <ap_int.h>

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
#pragma HLS INLINE
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
class binary_tower_t {
    ap_uint<W> raw;

    explicit binary_tower_t(ap_uint<W> raw) : raw(raw) {}

    binary_tower_t operator+(binary_tower_t &other) {
        return binary_tower_t(raw ^ other.raw);
    }

    binary_tower_t operator-(binary_tower_t &other) {
        return binary_tower_t(raw ^ other.raw);
    }

    binary_tower_t operator*(binary_tower_t &other) {
        return binary_tower_t(b_mul(raw, other.raw));
    }

    binary_tower_t square() {
        return binary_tower_t(b_sqr(raw));
    }

    binary_tower_t inverse() {
        return binary_tower_t(b_inv(raw));
    }

    std::string to_string() {
        return raw.to_string();
    }

    static ap_uint<W> b_mul_alpha(ap_uint<W> a) {
        if constexpr (W == 1) {
            return a;
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            return (a0 ^ b_mul_alpha(a1), a1);
        }
    }

    static ap_uint<W> b_mul(ap_uint<W> a, ap_uint<W> b) {
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

    static ap_uint<W> b_sqr(ap_uint<W> a) {
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

    static ap_uint<W> b_inv(ap_uint<W> a) {
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
};
