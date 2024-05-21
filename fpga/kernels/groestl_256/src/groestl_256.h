#pragma once
#include <hls_stream.h>
#include "state_matrix.h"

// -- [CONSTANTS] -----------------------------------------------------------------------------------------------------
static constexpr int N_BYTES_IN_BLOCK = 64;
static constexpr int MAX_MINI_BATCH_SIZE = 32;
static constexpr int MAX_BURST_SIZE = 16;

static constexpr int TEST_N_MSGS = 32;
static constexpr int TEST_N_BLOCKS = 128;
// --------------------------------------------------------------------------------------------------------------------

// -- [TYPES] ---------------------------------------------------------------------------------------------------------
using block_t = ap_uint<512>;
using hash_t = ap_uint<256>;
using word_t = ap_uint<8>;
using state_t = state_matrix_t<8, 8, 8>;

struct msg_pkt_t {
    block_t msg;
    bool first;
    bool last;
};

struct compress_pkt_t {
    state_t msg;
    state_t state;
    bool last;
};

using msg_cache_t =  msg_pkt_t[MAX_MINI_BATCH_SIZE][MAX_BURST_SIZE];
// --------------------------------------------------------------------------------------------------------------------

void kernel_m_axi_groestl_256(
        block_t *gmem_msg,
        hash_t *gmem_hash,
        int msg_size,
        int batch_size
);
