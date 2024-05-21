#include <iostream>
#include <cassert>
#include <xrt.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_aie.h>
#include <xrt/xrt_graph.h>
#include <xrt/xrt_uuid.h>
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>

struct hash_256_t {
    unsigned char bytes[32];

    void print() {
        for (int i = 0; i < 32; i++) {
            printf("%02x ", bytes[i]);
        }
        printf("\n");
    }
};

static xrt::uuid init_device(xrt::device &device) {
    xrt::uuid xclbin_uuid = device.load_xclbin("hw.xclbin");
    return xclbin_uuid;
}

int main() {
    constexpr uint64_t MSG_SIZE = 8192;
    constexpr uint32_t BATCH_SIZE = 32;

    xrt::device device = xrt::device(0);
    xrt::uuid xclbin_uuid = init_device(device);

    xrt::kernel k_hash = xrt::kernel(device, xclbin_uuid, "kernel_m_axi_groestl_256");

    auto k_hash_gmem_msg_bank_group = k_hash.group_id(0);
    auto k_hash_gmem_hash_bank_group = k_hash.group_id(1);

    auto msg_buffer = xrt::bo(device, MSG_SIZE * BATCH_SIZE, k_hash_gmem_msg_bank_group);
    auto hash_buffer = xrt::bo(device, sizeof(hash_256_t) * BATCH_SIZE, k_hash_gmem_hash_bank_group);

    static char msgs[BATCH_SIZE][MSG_SIZE] = {0};
    for (int i = 0; i < BATCH_SIZE; i++) {
        for (int j = 0; j < MSG_SIZE; j++) {
            msgs[i][j] = (j + i) % 256;
        }
    }

    msg_buffer.write(msgs);
    msg_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    auto start = std::chrono::high_resolution_clock::now();
    auto run = k_hash(msg_buffer, hash_buffer, MSG_SIZE, BATCH_SIZE);
    run.wait();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(stop - start).count();
    double total_gigabytes = static_cast<double>(MSG_SIZE * BATCH_SIZE) / (1ULL << 30);
    double throughput = total_gigabytes / duration;
    std::cout << "Throughput: " << throughput << " GiB/s" << std::endl;

    hash_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    static hash_256_t hashes[BATCH_SIZE] = {0};
    hash_buffer.read(hashes);
}