#include <hls_task.h>
#include <hls_streamofblocks.h>
#include "groestl_256.h"

// -- [UTIL] ----------------------------------------------------------------------------------------------------------
template<int W>
static ap_uint<W> reverse_bytes(ap_uint<W> x) {
#pragma HLS INLINE
    static_assert(W % 8 == 0);
    ap_uint<W> tmp;
    for (int i = 0; i < W / 8; i++) {
        tmp((W - 1) - 8 * i, (W - 8) - 8 * i) = x(7 + 8 * i, 8 * i);
    }
    return tmp;
}
// --------------------------------------------------------------------------------------------------------------------

// -- [COMPRESS] ------------------------------------------------------------------------------------------------------
static word_t sbox(word_t x) {
#pragma HLS INLINE
    const word_t lut[256] = {
            0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
            0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
            0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
            0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
            0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
            0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
            0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
            0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
            0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
            0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
            0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
            0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
            0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
            0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
            0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
            0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };
#pragma HLS BIND_STORAGE variable=lut type=rom_np impl=lutram
    return lut[x];
}

static word_t mul2(word_t x) {
#pragma HLS INLINE
    word_t tmp;
    tmp[0] = x[7];
    tmp[1] = x[7] ^ x[0];
    tmp[2] = x[1];
    tmp[3] = x[7] ^ x[2];
    tmp[4] = x[7] ^ x[3];
    tmp[5] = x[4];
    tmp[6] = x[5];
    tmp[7] = x[6];
    return tmp;
}

static state_t mix_bytes(state_t state) {
#pragma HLS INLINE
    state_t ret;
    for (int i = 0; i < 8; i++) {
        hls::vector<word_t, 8> b = state.get_col(i);
        hls::vector<word_t, 8> b2;
        hls::vector<word_t, 8> b4;
        for (int j = 0; j < 8; j++) {
            b2[j] = mul2(b[j]);
            b4[j] = mul2(b2[j]);
        }
        word_t t1 = b2[0] ^ b2[2] ^ b[5] ^ b4[7] ^ b[7];
        word_t t2 = b2[1] ^ b[4] ^ b4[6] ^ b[6] ^ b2[7];
        word_t t3 = b[0] ^ b4[2] ^ b[2] ^ b2[3] ^ b2[5];
        word_t t4 = b[1] ^ b4[3] ^ b[3] ^ b2[4] ^ b2[6];
        word_t t5 = b4[0] ^ b[0] ^ b2[3] ^ b4[5];
        word_t t6 = b4[1] ^ b4[4] ^ b[4] ^ b2[7];
        word_t t7 = b4[1] ^ b[1] ^ b2[4];
        word_t t8 = b2[0] ^ b4[5] ^ b[5];
        word_t t9 = b[2] ^ b2[5];
        word_t ta = b2[1] ^ b[6];
        word_t tb = b[3] ^ b2[6];
        word_t tc = b2[2] ^ b[7];

        ret[0][i] = t1 ^ t2 ^ t9 ^ b4[3] ^ b4[4];
        ret[1][i] = t1 ^ t5 ^ ta ^ tb ^ b4[4];
        ret[2][i] = t2 ^ t5 ^ t7 ^ tc;
        ret[3][i] = t1 ^ t3 ^ t7 ^ b4[6];
        ret[4][i] = t3 ^ t4 ^ ta ^ b4[0] ^ b4[7];
        ret[5][i] = t4 ^ t6 ^ t9 ^ tc ^ b4[0];
        ret[6][i] = t3 ^ t6 ^ t8 ^ tb;
        ret[7][i] = t2 ^ t4 ^ t8 ^ b4[2];
    }
    return ret;
}

static state_t add_round_constant_p(state_t state, word_t round) {
#pragma HLS INLINE
    for (int i = 0; i < 8; i++) {
        state[0][i] ^= word_t(i << 4) ^ round;
    }
    return state;
}

static state_t add_round_constant_q(state_t state, word_t round) {
#pragma HLS INLINE
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 8; i++) {
            state[j][i] ^= word_t(0xff);
        }
    }
    for (int i = 0; i < 8; i++) {
        state[7][i] ^= word_t(i << 4) ^ word_t(0xff) ^ round;
    }
    return state;
}

static state_t sub_bytes(state_t state) {
#pragma HLS INLINE
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            state[i][j] = sbox(state[i][j]);
        }
    }
    return state;
}

static state_t shift_bytes_p(state_t state) {
#pragma HLS INLINE
    state_t ret;
    int rc[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    ret = state.rotl_rows(rc);
    return ret;
}

static state_t shift_bytes_q(state_t state) {
#pragma HLS INLINE
    state_t ret;
    int rc[8] = {1, 3, 5, 7, 0, 2, 4, 6};
    ret = state.rotl_rows(rc);
    return ret;
}

static state_t inner_round_p(state_t state, int round) {
#pragma HLS PIPELINE II=1
    state_t ret = add_round_constant_p(state, round);
    ret = sub_bytes(ret);
    ret = shift_bytes_p(ret);
    ret = mix_bytes(ret);
    return ret;
}

static state_t inner_round_q(state_t state, int round) {
#pragma HLS PIPELINE II=1
    state_t ret = add_round_constant_q(state, round);
    ret = sub_bytes(ret);
    ret = shift_bytes_q(ret);
    ret = mix_bytes(ret);
    return ret;
}

static state_t compress_p(state_t state) {
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
    state_t ret = state;
    for (int i = 0; i < 10; i++) {
        ret = inner_round_p(ret, i);
    }
    return ret;
}

static state_t compress_q(state_t state) {
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
    state_t ret = state;
    for (int i = 0; i < 10; i++) {
        ret = inner_round_q(ret, i);
    }
    return ret;
}

static state_t compress(state_t h, state_t m) {
#pragma HLS PIPELINE II=1
    return compress_p(m ^ h) ^ compress_q(m) ^ h;
}
// --------------------------------------------------------------------------------------------------------------------

// -- [TASKS] ---------------------------------------------------------------------------------------------------------
static void dispatch_unit(
        hls::stream<msg_pkt_t> &msg_in,
        hls::stream<state_t> &state_in,
        hls::stream<compress_pkt_t> &compress_out
) {
#pragma HLS PIPELINE II=1
    state_t iv;
    iv[6][7] = 0x01;

    msg_pkt_t msg_pkt = msg_in.read();
    compress_pkt_t pkt;
    pkt.msg = state_t(reverse_bytes(msg_pkt.msg), state_t::COL_ORDER);
    pkt.last = msg_pkt.last;
    if (msg_pkt.first) {
        pkt.state = iv;
    } else {
        pkt.state = state_in.read();
    }
    compress_out << pkt;
}

static void compress_unit(
        hls::stream<compress_pkt_t> &in,
        hls::stream<state_t> &state_out,
        hls::stream<state_t> &final_out
) {
#pragma HLS PIPELINE II=1
    compress_pkt_t pkt = in.read();
    state_t tmp = compress(pkt.state, pkt.msg);
    if (pkt.last) {
        final_out << tmp;
    } else {
        state_out << tmp;
    }
}

static void output_unit(
        hls::stream<state_t> &final_in,
        hls::stream<hash_t> &hash_out
) {
#pragma HLS PIPELINE II=1
    state_t x = final_in.read();
    x = compress_p(x) ^ x;
    hash_t tmp = x.to_uint(state_t::COL_ORDER)(255, 0);
    hash_out << reverse_bytes(tmp);
}
// --------------------------------------------------------------------------------------------------------------------

// -- [IO] ------------------------------------------------------------------------------------------------------------
static uint32_t n_full_mini_batches(uint64_t batch_size) {
    return batch_size / MAX_MINI_BATCH_SIZE;
}

static uint32_t n_blocks(uint64_t msg_size) {
    return ((msg_size + N_BYTES_IN_BLOCK - 1) & ~(N_BYTES_IN_BLOCK - 1)) / N_BYTES_IN_BLOCK;
}

static uint32_t n_bursts(uint64_t msg_size) {
    return ((n_blocks(msg_size) + MAX_BURST_SIZE - 1) & ~(MAX_BURST_SIZE - 1)) / MAX_BURST_SIZE;
}

static uint32_t n_full_blocks(uint64_t msg_size) {
    return msg_size / N_BYTES_IN_BLOCK;
}

static uint32_t n_bytes_left_over(uint64_t msg_size) {
    return msg_size - n_full_blocks(msg_size) * N_BYTES_IN_BLOCK;
}

static bool require_extra_block(uint64_t msg_size) {
    return n_bytes_left_over(msg_size) > 55;
}

static uint32_t map_3d_to_1d(
        uint32_t mini_batch_idx, uint32_t msg_idx, uint32_t block_idx,
        uint32_t msg_idx_max, uint32_t block_idx_max
) {
#pragma HLS INLINE
    return block_idx + block_idx_max * (msg_idx + msg_idx_max * mini_batch_idx);
}

static void read_mini_batch(
        block_t *gmem,
        hls::stream<msg_pkt_t> &msg_out,
        uint32_t mini_batch_idx,
        uint64_t msg_size
) {
    const uint32_t n_full_bursts_per_msg = n_blocks(msg_size) / MAX_BURST_SIZE;
    const uint32_t n_blocks_left_over = n_blocks(msg_size) % MAX_BURST_SIZE;

    for (uint32_t burst_idx = 0; burst_idx < n_full_bursts_per_msg; burst_idx++) {
        for (uint32_t msg_idx = 0; msg_idx < MAX_MINI_BATCH_SIZE; msg_idx++) {
            burst_loop: for (uint32_t block_idx = 0; block_idx < MAX_BURST_SIZE; block_idx++) {
#pragma HLS PIPELINE II=1
            uint32_t idx = map_3d_to_1d(mini_batch_idx, msg_idx, burst_idx * MAX_BURST_SIZE + block_idx, MAX_MINI_BATCH_SIZE, n_blocks(msg_size));
            msg_pkt_t pkt;
            pkt.msg = gmem[idx];
            pkt.first = (burst_idx == 0) && (block_idx == 0);
            pkt.last = (n_blocks_left_over == 0) && (burst_idx == n_full_bursts_per_msg - 1) && (block_idx == MAX_BURST_SIZE - 1);
            msg_out << pkt;
        }
        }
    }

    if (n_blocks_left_over) {
        for (uint32_t msg_idx = 0; msg_idx < MAX_MINI_BATCH_SIZE; msg_idx++) {
            partial_loop: for (uint32_t block_idx = 0; block_idx < n_blocks_left_over; block_idx++) {
#pragma HLS PIPELINE II=1
            uint32_t idx = map_3d_to_1d(mini_batch_idx, msg_idx, n_full_bursts_per_msg * MAX_BURST_SIZE + block_idx, MAX_MINI_BATCH_SIZE, n_blocks(msg_size));
            msg_pkt_t pkt;
            pkt.msg = gmem[idx];
            pkt.first = (n_full_bursts_per_msg == 0) && (block_idx == 0);
            pkt.last = (block_idx == n_blocks_left_over - 1);
            msg_out << pkt;
        }
        }
    }
}

static void msg_reader(
        block_t *gmem,
        hls::stream<msg_pkt_t> &msg_out,
        uint32_t msg_size,
        uint32_t batch_size
) {
    for (uint32_t mini_batch_idx = 0; mini_batch_idx < n_full_mini_batches(batch_size); mini_batch_idx++) {
        read_mini_batch(gmem, msg_out, mini_batch_idx, msg_size);
    }
}

static void msg_transposer_i(
        hls::stream<msg_pkt_t> &msg_in,
        hls::stream_of_blocks<msg_cache_t> &msg_cache_out,
        uint32_t msg_size,
        uint32_t batch_size
) {
    const uint32_t n_iters = n_bursts(msg_size) * n_full_mini_batches(batch_size);
    for (uint32_t i = 0; i < n_iters; i++) {
        hls::write_lock<msg_cache_t> msg_cache(msg_cache_out);
        for (uint32_t msg_idx = 0; msg_idx < MAX_MINI_BATCH_SIZE; msg_idx++) {
            for (uint32_t block_idx = 0; block_idx < MAX_BURST_SIZE; block_idx++) {
#pragma HLS PIPELINE II=1
                msg_pkt_t tmp = msg_in.read();
                msg_cache[msg_idx][block_idx] = tmp;
                if (tmp.last) {
                    break;
                }
            }
        }
    }
}

static void msg_transposer_o(
        hls::stream_of_blocks<msg_cache_t> &msg_cache_in,
        hls::stream<msg_pkt_t> &msg_out,
        uint64_t msg_size,
        uint32_t batch_size
) {
    const uint32_t n_iters = n_bursts(msg_size) * n_full_mini_batches(batch_size);
    for (int i = 0; i < n_iters; i++) {
        hls::read_lock<msg_cache_t> msg_cache(msg_cache_in);
        bool last = false;
        for (uint32_t block_idx = 0; block_idx < MAX_BURST_SIZE; block_idx++) {
            for (uint32_t msg_idx = 0; msg_idx < MAX_MINI_BATCH_SIZE; msg_idx++) {
#pragma HLS PIPELINE II=1
                msg_pkt_t tmp = msg_cache[msg_idx][block_idx];
                last = tmp.last;
                msg_out << tmp;
            }
            if (last) {
                break;
            }
        }
    }
}

static void msg_padder(
        hls::stream<msg_pkt_t> &msg_in,
        hls::stream<msg_pkt_t> &msg_out,
        uint64_t msg_size,
        uint32_t batch_size
) {
    const uint32_t n_iters = n_bursts(msg_size) * n_full_mini_batches(batch_size) * MAX_BURST_SIZE;
    const uint32_t bytes_left_over = n_bytes_left_over(msg_size);
    for (int i = 0; i < n_iters; i++) {
        bool last = false;
        for (uint32_t msg_idx = 0; msg_idx < MAX_MINI_BATCH_SIZE; msg_idx++) {
#pragma HLS PIPELINE II=1
            msg_pkt_t pkt = msg_in.read();
            last = pkt.last;
            pkt.last = false;
            msg_out << pkt;
        }
        if (last) {
            // TODO: currently always inserts extra block.
            //       therefore the msg size must be in multiples of 64 bytes.
            for (uint32_t i = 0; i < MAX_MINI_BATCH_SIZE; i++) {
#pragma HLS PIPELINE II=1
                block_t padding = 0;
                padding(7, 0) = 0x80;
                ap_uint<64> n_blocks_bits = n_blocks(msg_size) + 1;
                padding(511, 504) = n_blocks_bits(7, 0);
                padding(503, 496) = n_blocks_bits(15, 8);
                padding(495, 488) = n_blocks_bits(23, 16);
                padding(487, 480) = n_blocks_bits(31, 24);
                msg_pkt_t pkt;
                pkt.msg = padding;
                pkt.first = false;
                pkt.last = true;
                msg_out << pkt;
            }
        }
    }
}

static void hash_writer(
        hash_t *gmem,
        hls::stream<hash_t> &hash_in,
        uint32_t batch_size
) {
    for (uint32_t i = 0; i < batch_size; i++) {
#pragma HLS PIPELINE II=1
        gmem[i] = hash_in.read();
    }
}
// --------------------------------------------------------------------------------------------------------------------

// -- [KERNEL] --------------------------------------------------------------------------------------------------------
void m_axi_groestl_256(
        block_t *gmem_msg,
        hash_t *gmem_hash,
        int msg_size,
        int batch_size
) {
#pragma HLS INTERFACE mode=m_axi depth=TEST_N_MSGS*TEST_N_BLOCKS num_read_outstanding=256 port=gmem_msg
#pragma HLS INTERFACE mode=m_axi depth=TEST_N_MSGS port=gmem_hash
#pragma HLS DATAFLOW
    hls_thread_local hls::stream<msg_pkt_t, 32> s_msg;
    hls_thread_local hls::stream<msg_pkt_t, 32> s_reordered;
    hls_thread_local hls::stream<msg_pkt_t> s_padded;
    hls_thread_local hls::stream<compress_pkt_t> s_compress;
    hls_thread_local hls::stream<state_t, 32> s_state;
    hls_thread_local hls::stream<state_t> s_final;
    hls_thread_local hls::stream<hash_t> s_hash;
    hls_thread_local hls::stream_of_blocks<msg_cache_t, 2> msg_cache;

    hls_thread_local hls::task t_dispatch_unit(dispatch_unit, s_padded, s_state, s_compress);
    hls_thread_local hls::task t_compress_unit(compress_unit, s_compress, s_state, s_final);
    hls_thread_local hls::task t_output_unit(output_unit, s_final, s_hash);

    msg_reader(gmem_msg, s_msg, msg_size, batch_size);
    msg_transposer_i(s_msg, msg_cache, msg_size, batch_size);
    msg_transposer_o(msg_cache, s_reordered, msg_size, batch_size);
    msg_padder(s_reordered, s_padded, msg_size, batch_size);
    hash_writer(gmem_hash, s_hash, batch_size);
}
// --------------------------------------------------------------------------------------------------------------------
