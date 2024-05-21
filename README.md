# open-binius
This repo contains building blocks for accelerating ZK proofs over binary fields.

## Hash Functions
A demo kernel is included with each hash function that conforms to the following API:

```c++
// hashes a batch_size of messages, each of the same msg_size
void hash(char *msg_buffer, char *hash_buffer, int msg_size, int batch_size);
```

The demo application demonstrates correctness, however it is intended for the user
to integrate the internal building blocks in their own FPGA-based design
(e.g. instantiate multiple cores).

Currently only the groestl_256 hash function is available. More hash functions coming soon!

### Demo Application Stats
Note that the demo applications are bottle-necked by PCIe or memory bandwidth.
Power is measured as total card power.

|             | LUTs    | Freq (MHz) | Throughput (GiB/s) | Power (W) |
|-------------|---------|------------|--------------------|-----------|
| groestl_256 | 122K    | 300        | 12.8189            | 31        |

### Hash Core Stats
Frequency achievable when instantiating the cores without data movers.
Power is measured just for the hash core.

|             | LUTs | Freq (MHz) | Throughput (GiB/s) | Power (W) |
|-------------|------|------------|--------------------|-----------|
| groestl_256 | 115K | 750        | 44.7               | 12        |

### Total Board Throughput
Achievable throughput when instantiating as many cores as possible, ignoring
memory bandwidth. On cards with HBM, this throughput is realistically achievable.

|             | U200        | U250      | U280        | U50         | U55C        |
|-------------|-------------|-----------|-------------|-------------|-------------|
| groestl_256 | 312.9 GiB/s | 447 GiB/s | 357.6 GiB/s | 223.5 GiB/s | 357.6 GiB/s |

### Running the Demo
The demo is targets any AMD Alveo card, any of the following will work:

```
AMD U200
AMD U250
AMD U280
AMD U50
AMD U55C
```

#### Install Deployment Prerequisites
Download and install the Xilinx Runtime (XRT) and the deployment target platform for your respective
device:

(NOTE: Tool version >= 2023.1 required)
https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/alveo.html

For example, assuming you have an U250 on Ubuntu 22.04, install the packages using apt:

```bash
# Download files
wget https://www.xilinx.com/bin/public/openDownload?filename=xrt_202310.2.15.225_22.04-amd64-xrt.deb
wget https://www.xilinx.com/bin/public/openDownload?filename=xilinx-u250-gen3x16-xdma_2023.1_2023_0507_2220-all.deb.tar.gz
# Extract tar
tar -xvf ./xilinx-u250-gen3x16-xdma_2023.1_2023_0507_2220-all.deb.tar.gz
# Install all deb files
sudo apt install ./xrt_202310.2.15.225_22.04-amd64-xrt.deb
cd xilinx-u250-gen3x16-xdma_2023.1_2023_0507_2220-all
sudo apt install ./*.deb
```

If this is your first time using XRT with your card please follow the instructions to update your card from the user guide:
https://docs.xilinx.com/r/en-US/ug1301-getting-started-guide-alveo-accelerator-cards

#### Compiling the xclbin
In addition to the deployment platform you need the development platform for your respective card, and
install the latest Vitis 2023.2 tools.

After installing the tools, we can finally compile the fpga image:

```bash
git clone https://github.com/ingonyama-zk/open-binius.git
source /tools/Xilinx/Vitis/2023.2/settings64.sh
source /opt/xilinx/xrt/setup.sh
cd open-binius
cd fpga/systems/groestl_256/
make ../../kernels/groestl_256/work/m_axi_groestl_256.xo
make work/xilinx_u250_gen3x16_xdma_4_1_202210_1/m_axi_groestl_256/hw/m_axi_groestl_256.xclbin
```

#### Compile and run the host application
Now you can run the demo host application.

```bash
make demo
```
