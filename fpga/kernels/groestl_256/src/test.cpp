#include "groestl_256.h"

int main() {
    char gmem_msg[TEST_N_MSGS][TEST_N_BLOCKS * N_BYTES_IN_BLOCK];
    hash_t gmem_hash[TEST_N_MSGS];

    for (int i = 0; i < TEST_N_MSGS; i++) {
        for (int j = 0; j < TEST_N_BLOCKS * N_BYTES_IN_BLOCK; j++) {
            gmem_msg[i][j] = (j + i) % 256;
        }
    }

    kernel_m_axi_groestl_256((block_t *) gmem_msg, gmem_hash, TEST_N_BLOCKS * N_BYTES_IN_BLOCK, TEST_N_MSGS);

    for (int i = 0; i < TEST_N_MSGS; i++) {
        std::cout << gmem_hash[i].to_string(16) << std::endl;
    }
    return 0;
}
