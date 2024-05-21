#pragma once

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
    ap_uint<W> b_mul(ap_uint<W> a, ap_uint<W> b) {
#pragma HLS INLINE
        if constexpr (W == 4) {
            const ap_uint<4> lut[256] = {
                    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,
                    0x0, 0x2, 0x3, 0x1, 0x8, 0xa, 0xb, 0x9, 0xc, 0xe, 0xf, 0xd, 0x4, 0x6, 0x7, 0x5,
                    0x0, 0x3, 0x1, 0x2, 0xc, 0xf, 0xd, 0xe, 0x4, 0x7, 0x5, 0x6, 0x8, 0xb, 0x9, 0xa,
                    0x0, 0x4, 0x8, 0xc, 0x9, 0xd, 0x1, 0x5, 0xe, 0xa, 0x6, 0x2, 0x7, 0x3, 0xf, 0xb,
                    0x0, 0x5, 0xa, 0xf, 0xd, 0x8, 0x7, 0x2, 0x6, 0x3, 0xc, 0x9, 0xb, 0xe, 0x1, 0x4,
                    0x0, 0x6, 0xb, 0xd, 0x1, 0x7, 0xa, 0xc, 0x2, 0x4, 0x9, 0xf, 0x3, 0x5, 0x8, 0xe,
                    0x0, 0x7, 0x9, 0xe, 0x5, 0x2, 0xc, 0xb, 0xa, 0xd, 0x3, 0x4, 0xf, 0x8, 0x6, 0x1,
                    0x0, 0x8, 0xc, 0x4, 0xe, 0x6, 0x2, 0xa, 0x7, 0xf, 0xb, 0x3, 0x9, 0x1, 0x5, 0xd,
                    0x0, 0x9, 0xe, 0x7, 0xa, 0x3, 0x4, 0xd, 0xf, 0x6, 0x1, 0x8, 0x5, 0xc, 0xb, 0x2,
                    0x0, 0xa, 0xf, 0x5, 0x6, 0xc, 0x9, 0x3, 0xb, 0x1, 0x4, 0xe, 0xd, 0x7, 0x2, 0x8,
                    0x0, 0xb, 0xd, 0x6, 0x2, 0x9, 0xf, 0x4, 0x3, 0x8, 0xe, 0x5, 0x1, 0xa, 0xc, 0x7,
                    0x0, 0xc, 0x4, 0x8, 0x7, 0xb, 0x3, 0xf, 0x9, 0x5, 0xd, 0x1, 0xe, 0x2, 0xa, 0x6,
                    0x0, 0xd, 0x6, 0xb, 0x3, 0xe, 0x5, 0x8, 0x1, 0xc, 0x7, 0xa, 0x2, 0xf, 0x4, 0x9,
                    0x0, 0xe, 0x7, 0x9, 0xf, 0x1, 0x8, 0x6, 0x5, 0xb, 0x2, 0xc, 0xa, 0x4, 0xd, 0x3,
                    0x0, 0xf, 0x5, 0xa, 0xb, 0x4, 0xe, 0x1, 0xd, 0x2, 0x8, 0x7, 0x6, 0x9, 0x3, 0xc
            };
#pragma HLS BIND_STORAGE variable=lut type=rom_np impl=lutram
            ap_uint<8> idx = (a, b);
            return lut[idx];
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            ap_uint<W / 2> b0 = b;
            ap_uint<W / 2> b1 = b >> W / 2;
            ap_uint<W / 2> z0 = b_mul(a0, b0);
            ap_uint<W / 2> z1 = b_mul(a0 ^ a1, b0 ^ b1);
            ap_uint<W / 2> z2 = b_mul(a1, b1);

            return ((z1 ^ z0 ^ z2) ^ (z2 << W / 4), z0 ^ z2);
        }
    }

    template<int W>
    ap_uint<W> b_inv(ap_uint<W> a) {
#pragma HLS INLINE
        if constexpr (W == 8) {
            const ap_uint<8> lut[256] = {
                    0x00, 0x01, 0x03, 0x02, 0x06, 0x0e, 0x04, 0x0f, 0x0d, 0x0a, 0x09, 0x0c, 0x0b, 0x08, 0x05, 0x07,
                    0x14, 0x67, 0x94, 0x7b, 0x10, 0x66, 0x9e, 0x7e, 0xd2, 0x81, 0x27, 0x4b, 0xd1, 0x8f, 0x2f, 0x42,
                    0x3c, 0xe6, 0xde, 0x7c, 0xb3, 0xc1, 0x4a, 0x1a, 0x30, 0xe9, 0xdd, 0x79, 0xb1, 0xc6, 0x43, 0x1e,
                    0x28, 0xe8, 0x9d, 0xb9, 0x63, 0x39, 0x8d, 0xc2, 0x62, 0x35, 0x83, 0xc5, 0x20, 0xe7, 0x97, 0xbb,
                    0x61, 0x48, 0x1f, 0x2e, 0xac, 0xc8, 0xbc, 0x56, 0x41, 0x60, 0x26, 0x1b, 0xcf, 0xaa, 0x5b, 0xbe,
                    0xef, 0x73, 0x6d, 0x5e, 0xf7, 0x86, 0x47, 0xbd, 0x88, 0xfc, 0xbf, 0x4e, 0x76, 0xe0, 0x53, 0x6c,
                    0x49, 0x40, 0x38, 0x34, 0xe4, 0xeb, 0x15, 0x11, 0x8b, 0x85, 0xaf, 0xa9, 0x5f, 0x52, 0x98, 0x92,
                    0xfb, 0xb5, 0xee, 0x51, 0xb7, 0xf0, 0x5c, 0xe1, 0xdc, 0x2b, 0x95, 0x13, 0x23, 0xdf, 0x17, 0x9f,
                    0xd3, 0x19, 0xc4, 0x3a, 0x8a, 0x69, 0x55, 0xf6, 0x58, 0xfd, 0x84, 0x68, 0xc3, 0x36, 0xd0, 0x1d,
                    0xa6, 0xf3, 0x6f, 0x99, 0x12, 0x7a, 0xba, 0x3e, 0x6e, 0x93, 0xa0, 0xf8, 0xb8, 0x32, 0x16, 0x7f,
                    0x9a, 0xf9, 0xe2, 0xdb, 0xed, 0xd8, 0x90, 0xf2, 0xae, 0x6b, 0x4d, 0xce, 0x44, 0xc9, 0xa8, 0x6a,
                    0xc7, 0x2c, 0xc0, 0x24, 0xfa, 0x71, 0xf1, 0x74, 0x9c, 0x33, 0x96, 0x3f, 0x46, 0x57, 0x4f, 0x5a,
                    0xb2, 0x25, 0x37, 0x8c, 0x82, 0x3b, 0x2d, 0xb0, 0x45, 0xad, 0xd7, 0xff, 0xf4, 0xd4, 0xab, 0x4c,
                    0x8e, 0x1c, 0x18, 0x80, 0xcd, 0xf5, 0xfe, 0xca, 0xa5, 0xec, 0xe3, 0xa3, 0x78, 0x2a, 0x22, 0x7d,
                    0x5d, 0x77, 0xa2, 0xda, 0x64, 0xea, 0x21, 0x3d, 0x31, 0x29, 0xe5, 0x65, 0xd9, 0xa4, 0x72, 0x50,
                    0x75, 0xb6, 0xa7, 0x91, 0xcc, 0xd5, 0x87, 0x54, 0x9b, 0xa1, 0xb4, 0x70, 0x59, 0x89, 0xd6, 0xcb
            };
#pragma HLS BIND_STORAGE variable=lut type=rom_np impl=lutram
            return lut[a];
        } else {
            ap_uint<W / 2> a0 = a;
            ap_uint<W / 2> a1 = a >> W / 2;
            ap_uint<W / 2> a0z1 = a0 ^ (a1 << W / 4);
            ap_uint<W / 2> delta = b_mul(a0, a0z1) ^ b_mul(a0, a0);
            ap_uint<W / 2> delta_inv = b_inv(delta);
            return (b_mul(delta_inv, a1), b_mul(delta_inv, a0z1));
        }
    }
}
