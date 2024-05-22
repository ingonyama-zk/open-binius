#include "vision_mark_32.h"
#include "binary_tower.h"

using state_t = vision_mark_32::state_t;

// TODO: replace with real round constants
static const ap_uint<32> RC[8][24]{
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
};

template<bool inv>
static ap_uint<32> linearized_affine(ap_uint<32> x) {
    // TODO:
    return x;
}

static state_t apply_inverse(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = binary_tower::b_inv(state.v[i]);
    }
    return state;
}

template<bool inv>
static state_t apply_linear(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = linearized_affine<inv>(state.v[i]);
    }
    return state;
}

static state_t apply_mds(state_t state) {
    // TODO:
    return state;
}

template<int R>
static state_t add_round_constant(state_t state) {
    for (int i = 0; i < 24; i++) {
        state.v[i] = binary_tower::b_add(state.v[i], RC[R][i]);
    }
    return state;
}

template<int R>
state_t round(state_t state) {
    state = apply_inverse(state);
    state = apply_linear<true>(state);
    state = add_round_constant<R>(apply_mds(state));
    state = apply_inverse(state);
    state = apply_linear<false>(state);
    state = add_round_constant<R>(apply_mds(state));
    return state;
}

state_t vision_mark_32::permute(vision_mark_32::state_t state) {
#pragma HLS PIPELINE II=1
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
