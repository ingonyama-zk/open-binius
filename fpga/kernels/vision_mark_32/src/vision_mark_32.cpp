#include "vision_mark_32.h"
#include "binary_tower.h"

struct state_t {
    binary_tower_t<32> v[24]{};

    state_t(ap_uint<768> x) {
        for (int i = 0; i < 24; i++) {
            v[i] = binary_tower_t<32>(x);
            x >>= 32;
        }
    }

    ap_uint<768> to_packed() {
        ap_uint<768> x = 0;
        for (int i = 0; i < 24; i++) {
            x |= v[23 - i].raw;
            x <<= 32;
        }
        return x;
    }
};

struct narrow_state_t {
    binary_tower_t<32> v[8];
};

static constexpr uint32_t RC_INIT[24] = {
        0x545e66a7, 0x073fdd58, 0x84362677, 0x95fe8565, 0x06269cd8, 0x9c17909e, 0xf1f0adee, 0x2694c698,
        0x94b2788f, 0x5eac14ad, 0x21677a78, 0x5755730b, 0x37cef9cf, 0x2fb31ffe, 0xfc0082ec, 0x609c12f0,
        0x102769ee, 0x4732860d, 0xf97935e0, 0x36e77c02, 0xba9e70df, 0x67b701d7, 0x829d77a4, 0xf6ec454d,
};

static constexpr uint32_t RC[16][24] = {
        {
                0x73fa03e1, 0x2551a651, 0x0541031f, 0xec1d59dc, 0x780f0b43, 0x04843f97, 0xd379e20b, 0xbe4d1f58,
                0xe4c2f8bb, 0xf29aee17, 0x228e51f2, 0x840ee72b, 0x0cb3b70e, 0xbacc914a, 0x490bfa7e, 0xc2c3018e,
                0xf6d13bd2, 0x6a4d1fe1, 0x4c2bc742, 0x4508cb46, 0xed63a2f1, 0x1802f859, 0xa00483b5, 0x56e12e78,
        },
        {
                0xbace7a4a, 0xc3a71400, 0x5a29b316, 0xeedc8d26, 0x51c14d7e, 0x2add9be8, 0xfec386f3, 0x630765dc,
                0xaef6c21b, 0x8c98fc1a, 0x3f9c4f9d, 0x399e7231, 0xba238b04, 0x787d67ec, 0x313ad8af, 0xdd622ac3,
                0xe0a8d9b8, 0xaa0e6306, 0x29864574, 0x67ee41da, 0x5502278d, 0xa92e20b1, 0x0fdc5328, 0x74dbee7a,
        },
        {
                0x8bd2f341, 0x59dc2758, 0x5146c720, 0x9df43021, 0x0d1c49ea, 0x81d4b0a5, 0xdbfae4d1, 0xc3153118,
                0x8a3aec81, 0xeacd8854, 0xdd4b2576, 0x7dd5cee9, 0x5e9e77b7, 0x6b6efd8d, 0xf11db400, 0x0e6258b7,
                0x37b3b6e3, 0xed884c61, 0xb948717c, 0x3d7554cf, 0x42419311, 0x422d11d3, 0x5392b2e7, 0x79c1591d,
        },
        {
                0x27df48ae, 0x948bc10e, 0xcb079dc1, 0xed183a37, 0x81dcc076, 0x24cb0490, 0x52d69fb8, 0x8b576666,
                0x7499fe4d, 0x8f879e34, 0xcb11780a, 0x5e876b29, 0xb72d96ab, 0xac28e621, 0x18aeca7e, 0xf3d30baf,
                0xc7fca53f, 0xf7b3281a, 0xdaf5559f, 0x27aca0b3, 0x9515d3ee, 0xa39fd2e1, 0xf61b52c2, 0x83ec5a0f,
        },
        {
                0x89841f23, 0x8bd0c3e1, 0xde2dd62b, 0x37799416, 0x558834c7, 0x939df560, 0xb1a9f457, 0xcb24dadb,
                0x4f702a2a, 0x65ad5822, 0x7ecf577d, 0x728b4092, 0x89e4fa7d, 0x10cd7556, 0x1de77ab7, 0x2374c530,
                0x95b289d4, 0xa6ad3862, 0xc6b1a233, 0xac501639, 0x3f6072a3, 0xf8ae7cc4, 0x622f4cf3, 0xf7ccf75b,
        },
        {
                0xaedf6aac, 0xd64356b2, 0x1cbba169, 0x688602ae, 0x6231b358, 0x1fba8b86, 0xf7b83f1c, 0x465c4050,
                0x4403e74c, 0x5f51c2f3, 0x1b114a4d, 0x8f32bf48, 0x87990cfc, 0x71b55cb1, 0x73083164, 0x2fbd58ae,
                0x59bbcbbf, 0xb0dc1eba, 0xf2f169ff, 0x54cc93e8, 0x975cfc83, 0xbefc67cf, 0xb841429b, 0xff127d64,
        },
        {
                0xd6561783, 0x88153c99, 0x1a04e141, 0x62631076, 0xb20b52a2, 0x1df18264, 0x05176f17, 0x505b2752,
                0x914a71aa, 0x1b6cf96d, 0x5a8b3b59, 0x3ab885cc, 0xed662f24, 0xa859f626, 0xd91136bb, 0x6da2d95b,
                0x043fd679, 0xb9e685e8, 0xdf796fa5, 0x53fc28a3, 0x0c15dc77, 0x079255d2, 0x3373a2a0, 0xfc6b012e,
        },
        {
                0xb3359ff0, 0xa471acdc, 0x6ad3e18a, 0x4f012f65, 0xebd4392f, 0x25d3af23, 0x7a68469c, 0xd479ea57,
                0xb55b6450, 0x86ef0a15, 0xeefd412f, 0xc8e98f30, 0x61e0c12d, 0x36c4680c, 0xe818ab96, 0xfcb765f2,
                0xcbb47fea, 0xc9e202a8, 0xc762caec, 0x366f08fd, 0x5e2f3a19, 0x242c8397, 0x638a0042, 0x63f1c9c5,
        },
        {
                0x4e28a23c, 0xdbe6f0db, 0x9cf4faeb, 0x2fde2616, 0x22dedea1, 0x08ef118e, 0xd7f16ae2, 0xa13b30a8,
                0x2ceb58c1, 0xca587d86, 0xf6d54fcd, 0x9cd9f3f5, 0x9b0f94a2, 0xdede0863, 0xa608eb2d, 0x4d3c4469,
                0x53784235, 0x4cf6aa1e, 0xcb6ec0d5, 0xf334b49e, 0xe5f7a67a, 0x989658a2, 0xa1a672ca, 0x6fb7eced,
        },
        {
                0x2bbdf7b8, 0xa8626256, 0xd95bf688, 0x43245a87, 0xc14af030, 0x28e5933a, 0x3aeb3e0d, 0x169f7dea,
                0x4cd4d1e4, 0x8db556b5, 0xdd1a49ea, 0xe64eff5d, 0x8bd56648, 0x2c3422be, 0x5cffb53f, 0x6b7aaa6e,
                0xc2a8d1af, 0x7e79bed4, 0xd0b08e51, 0x1861ba54, 0xb7d3c6b4, 0x6a9bd7ca, 0x129d3aa5, 0x809e9413,
        },
        {
                0x52538d7d, 0xdd441420, 0x38a2e2d5, 0xccd05f31, 0x2a49f3a6, 0xe533cc9b, 0xa18de92e, 0x495f684a,
                0x0028e3ae, 0xd4072861, 0x370fd7a3, 0x728224bc, 0xa8b6b3d7, 0xdada7046, 0xea9e71df, 0x914f7d53,
                0x9b796ac9, 0xe7f61a69, 0x67a68f71, 0x7eb15ce6, 0xeb9af9e1, 0xa75f54b1, 0x59210427, 0x75093378,
        },
        {
                0x27866fea, 0x3bd84dca, 0x681d1d3a, 0xe7fb7496, 0x86fd9bf8, 0xc1f28786, 0xb3f17a06, 0x60c43dbe,
                0x16fee1be, 0xa8407554, 0xca909e3b, 0xb1fc461c, 0xd84d663e, 0x2e7d669b, 0x5b5b5a56, 0x6c53d090,
                0x236707a6, 0x7f1f4e97, 0xe95b23f3, 0x8cd1e3dd, 0x928f3212, 0x9c7c1c20, 0x00eeebe3, 0xc0572f52,
        },
        {
                0xd1504060, 0x005d8a96, 0x058e317a, 0x30d9d3c6, 0xa585af56, 0x084c5111, 0x498da85e, 0x0149987d,
                0xe130153b, 0x817cc725, 0x75f726b1, 0x23941339, 0x1f26e9dd, 0xdb013723, 0x81f36069, 0xe4167ba1,
                0x50d59f82, 0xbf011350, 0x3ae71f42, 0x9966d041, 0xdbe09577, 0xa830b8f0, 0x0c018c2d, 0x08beab4f,
        },
        {
                0x20898252, 0xac8aa337, 0x5c5bbcad, 0x2fa58f41, 0xf2446068, 0xfff46a79, 0x0b1980d8, 0x01b14c53,
                0x4e432072, 0xfc610a31, 0x80ba5531, 0xc14507a5, 0x2433c5d2, 0x8a461cf3, 0x187849cd, 0x3d4f51e8,
                0x3d9cd125, 0xe15e09ca, 0x8c6287c6, 0xfa0ec2f4, 0x65435f29, 0xd33a4f3d, 0xd61bb963, 0x991005f9,
        },
        {
                0x00d80bd4, 0x3d8b3d56, 0xcc18a7a9, 0x0105e9bb, 0x71f0e736, 0x4cc71fa4, 0x1a2ec96b, 0xe1b8b093,
                0x329232ab, 0xb4285526, 0x02326fe9, 0xe79accab, 0xd893b618, 0x9bd74bd5, 0x2062577c, 0x94f82da9,
                0xb551d97a, 0x862483f0, 0x5f8e4e3e, 0x098d5e44, 0xbe326102, 0x4f5f050e, 0x1bd571d5, 0xcdd8e583,
        },
        {
                0x1b525e1b, 0x1cccb851, 0x45b3c777, 0x63cc9153, 0xdfa0fd4a, 0x0cf20c06, 0x72fdd2f3, 0xf9b6f564,
                0x9552a62b, 0x1e848099, 0x3ba1a5a6, 0x17ff06e0, 0x8cae82ed, 0xb5b29fbc, 0x9322d5a6, 0x77f40c4c,
                0x0843ce60, 0x86ddb97f, 0xe5a12a04, 0x9bd65cd6, 0x1b16bea6, 0xf4066cee, 0xdcb3c788, 0x499b6483,
        },
};

// B(x) = 0x9fd55d88 x^4 + 0x391c8e32 x^2 + 0xdb43e603 x + 0x7cf0bc6c
static const ap_uint<32> B_LINEAR[32] = {
        0x7d8a35b9, 0xafd394ec, 0x0a5e7f50, 0x79326ede, 0x54dd7312, 0x4d9e11ed, 0x8360fa18, 0x729faf77,
        0x6943d917, 0xe3ce8313, 0xaacc8bbf, 0xc3e671a4, 0xbfa620f6, 0x85ad527d, 0x5a12cb53, 0x9f170345,
        0x6feef5fa, 0xebaa9b54, 0xd17de56f, 0x595a23bd, 0x64a204bd, 0x2bdddefd, 0x881e595a, 0xabcc6a47,
        0x2c0d1240, 0x14ee84e7, 0x8c04ad9c, 0x24b7eba9, 0x8069fd1a, 0x0809b759, 0xabd63eb1, 0xa2e6ae06,
};

static const ap_uint<32> B_CONST = 0x7cf0bc6c;

static const ap_uint<32> B_INV_LINEAR[32] = {
        0xef6bbaea, 0x4db1dec6, 0x90d25937, 0x9f94bac7, 0x6b98e587, 0x60682489, 0xef03f140, 0x22975eaa,
        0x8ab8c3ee, 0x029af7e4, 0xf657c0bd, 0x5275cd96, 0xd4704891, 0x511ff074, 0xe789c4f2, 0x2efd322b,
        0xb79dbde2, 0x969a22d9, 0x8cf40b41, 0x044ea0d7, 0x0cfd27c5, 0xd4ec9d06, 0xb144aabd, 0xb8408461,
        0x4ba31560, 0xdbb42264, 0x30d20351, 0xb25077a6, 0xb6447d2a, 0x854c2cb6, 0x70a5125a, 0x1d472bf1,
};

static const ap_uint<32> B_INV_CONST = 0x9fa712f2;

template<bool inv>
static binary_tower_t<32> permute_lane(binary_tower_t<32> x) {
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
    ap_uint<32> y = x.inv().raw;
    if constexpr (inv) {
        ap_uint<32> res = 0;
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                res[i] ^= y[j] & B_INV_LINEAR[j][i];
            }
        }
        return binary_tower_t<32>((res ^ B_INV_CONST));
    } else {
        ap_uint<32> res = 0;
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                res[i] ^= y[j] & B_LINEAR[j][i];
            }
        }
        return binary_tower_t<32>((res ^ B_CONST));
    }
}

template<bool inv>
static state_t permute(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = permute_lane<inv>(state.v[i]);
    }
    return state;
}

static state_t add_rc_init(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = state.v[i] + binary_tower_t<32>(RC_INIT[i]);
    }
    return state;
}

template<int ROUND>
static state_t add_rc(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = state.v[i] + binary_tower_t<32>(RC[ROUND][i]);
    }
    return state;
}

template<int coset>
narrow_state_t ifft_8(narrow_state_t x) {
    static constexpr uint32_t twiddles[3][7] = {
            {0x00, 0x00, 0x0d, 0x00, 0x02, 0x04, 0x06},
            {0x02, 0x0f, 0x02, 0x08, 0x0a, 0x0c, 0x0e},
            {0x64, 0x51, 0x5c, 0x10, 0x12, 0x14, 0x16}
    };
    x.v[1] += x.v[0];
    x.v[0] += x.v[1] * binary_tower_t<32>(twiddles[coset][3]);
    x.v[3] += x.v[2];
    x.v[2] += x.v[3] * binary_tower_t<32>(twiddles[coset][4]);
    x.v[5] += x.v[4];
    x.v[4] += x.v[5] * binary_tower_t<32>(twiddles[coset][5]);
    x.v[7] += x.v[6];
    x.v[6] += x.v[7] * binary_tower_t<32>(twiddles[coset][6]);
    x.v[2] += x.v[0];
    x.v[0] += x.v[2] * binary_tower_t<32>(twiddles[coset][1]);
    x.v[3] += x.v[1];
    x.v[1] += x.v[3] * binary_tower_t<32>(twiddles[coset][1]);
    x.v[6] += x.v[4];
    x.v[4] += x.v[6] * binary_tower_t<32>(twiddles[coset][2]);
    x.v[7] += x.v[5];
    x.v[5] += x.v[7] * binary_tower_t<32>(twiddles[coset][2]);
    x.v[4] += x.v[0];
    x.v[0] += x.v[4] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[5] += x.v[1];
    x.v[1] += x.v[5] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[6] += x.v[2];
    x.v[2] += x.v[6] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[7] += x.v[3];
    x.v[3] += x.v[7] * binary_tower_t<32>(twiddles[coset][0]);
    return x;
}

template<int coset>
narrow_state_t fft_8(narrow_state_t x) {
    static constexpr uint32_t twiddles[3][7] = {
            {0x66, 0x5e, 0x53, 0x18, 0x1a, 0x1c, 0x1e},
            {0xb8, 0xe3, 0xee, 0x20, 0x22, 0x24, 0x26},
            {0xba, 0xec, 0xe1, 0x28, 0x2a, 0x2c, 0x2e}
    };
    x.v[0] += x.v[4] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[4] += x.v[0];
    x.v[1] += x.v[5] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[5] += x.v[1];
    x.v[2] += x.v[6] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[6] += x.v[2];
    x.v[3] += x.v[7] * binary_tower_t<32>(twiddles[coset][0]);
    x.v[7] += x.v[3];
    x.v[0] += x.v[2] * binary_tower_t<32>(twiddles[coset][1]);
    x.v[2] += x.v[0];
    x.v[1] += x.v[3] * binary_tower_t<32>(twiddles[coset][1]);
    x.v[3] += x.v[1];
    x.v[4] += x.v[6] * binary_tower_t<32>(twiddles[coset][2]);
    x.v[6] += x.v[4];
    x.v[5] += x.v[7] * binary_tower_t<32>(twiddles[coset][2]);
    x.v[7] += x.v[5];
    x.v[0] += x.v[1] * binary_tower_t<32>(twiddles[coset][3]);
    x.v[1] += x.v[0];
    x.v[2] += x.v[3] * binary_tower_t<32>(twiddles[coset][4]);
    x.v[3] += x.v[2];
    x.v[4] += x.v[5] * binary_tower_t<32>(twiddles[coset][5]);
    x.v[5] += x.v[4];
    x.v[6] += x.v[7] * binary_tower_t<32>(twiddles[coset][6]);
    x.v[7] += x.v[6];
    return x;
}

static state_t mds(state_t state) {
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
    static constexpr uint32_t TOP_TWIDDLES[3] = {0x07, 0x6a, 0x02};
    narrow_state_t a;
    narrow_state_t b;
    narrow_state_t c;
    for (int i = 0; i < 8; i++) {
        a.v[i] = state.v[i];
        b.v[i] = state.v[i + 8];
        c.v[i] = state.v[i + 16];
    }
    a = ifft_8<0>(a);
    b = ifft_8<1>(b);
    c = ifft_8<2>(c);

    narrow_state_t tmp_0 = a;
    narrow_state_t tmp_1 = b;
    narrow_state_t x;
    narrow_state_t y;
    narrow_state_t z;

    for (int i = 0; i < 8; i++) {
        x.v[i] = binary_tower_t<32>(TOP_TWIDDLES[0]) * b.v[i];
        c.v[i] += x.v[i] + a.v[i];
        y.v[i] = binary_tower_t<32>(TOP_TWIDDLES[1]) * b.v[i];
        z.v[i] = binary_tower_t<32>(TOP_TWIDDLES[2]) * c.v[i];
        a.v[i] += x.v[i] + b.v[i] + c.v[i];
        b.v[i] = tmp_0.v[i] + y.v[i] + z.v[i];
        c.v[i] = b.v[i] + tmp_1.v[i];
    }

    a = fft_8<0>(a);
    b = fft_8<1>(b);
    c = fft_8<2>(c);
    for (int i = 0; i < 8; i++) {
        state.v[i] = a.v[i];
        state.v[8 + i] = b.v[i];
        state.v[16 + i] = c.v[i];
    }
    return state;
}

template<int ROUND>
state_t round(state_t state) {
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
    state = permute<true>(state);
    state = mds(state);
    state = add_rc<2 * ROUND>(state);
    state = permute<false>(state);
    state = mds(state);
    state = add_rc<2 * ROUND + 1>(state);
    return state;
}

static state_t update(state_t state) {
    state = add_rc_init(state);
    state = round<0>(state);
    state = round<1>(state);
    state = round<2>(state);
    state = round<3>(state);
    state = round<4>(state);
    state = round<5>(state);
    state = round<6>(state);
    state = round<7>(state);
    return state;
}

ap_uint<768> vision_mark_32(ap_uint<768> x) {
#pragma HLS INTERFACE MODE=ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
    state_t state(x);
    return update(state).to_packed();
}

//ap_uint<32> dummy(ap_uint<32> x, ap_uint<32> y) {
//#pragma HLS INTERFACE MODE=ap_ctrl_none port=return
//#pragma HLS PIPELINE II=1
//    return binary_tower::b_mul(x, y);
//}

//ap_uint<32> derp(ap_uint<32> a) {
//#pragma HLS INTERFACE MODE=ap_ctrl_none port=return
//#pragma HLS PIPELINE II=1
//    return binary_tower_32b_mul_alpha(a);
//}