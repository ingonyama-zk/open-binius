# Open-Binius
This repo contains open-source hardware IPs for accelerating ZK proofs over binary fields.

Binius is a proof system developed by Irreducible. For more information, visit [binius.xyz](http://binius.xyz). 

Open-Binius is a community effort led by Ingonyama. 



<p align="center">
  <img alt="obvb" width="600" height="300" src="https://github.com/ingonyama-zk/open-binius/assets/2446179/442d77cf-a5b6-4df0-9aea-649edb11c77a"/>
</p>
<p align="center">

<div align="center">credit: Vitalik Buterin</div>


## Background 
For background see [Vitalik blog](https://vitalik.eth.limo/general/2024/04/29/binius.html) on highly efficient proofs over binary fields or our [slides](https://github.com/ingonyama-zk/open-binius/blob/main/Binius%20May2024%20v3.pdf).

## Binary Tower Field Arithmetic
We are implementing primitives for binary tower field arithmetic on FPGA.

Work-in-progress results:

|               | LUTs | Freq (MHz) | 
|---------------|------|------------|
| 32b tower MUL (this code) | **502** | **880MHz**     |
| 32b tower MUL ([vm-32 paper](https://eprint.iacr.org/archive/2024/633/20240501:125118)) | 521  | 378MHz     |

|               | LUTs | Freq (MHz) | 
|---------------|------|------------|
| 32b tower INV (this code) | **785**  | **880MHz**     |
| 32b tower INV ([vm-32 paper](https://eprint.iacr.org/archive/2024/633/20240501:125118)) | 821  | 280MHz     |

## Hash Functions
A demo kernel is included with each hash function that conforms to the following API:

```c++
// hashes a batch_size of messages, each of the same msg_size
void hash(char *msg_buffer, char *hash_buffer, int msg_size, int batch_size);
```

The demo application demonstrates correctness, however it is intended for the user
to integrate the internal building blocks in their own FPGA-based design
(e.g. instantiate multiple cores).

### Demo Application Stats
Note that the demo applications are bottle-necked by PCIe or memory bandwidth.
Power is measured as total card power.

|             | LUTs    | Freq (MHz) | Throughput (GiB/s) | Power (W) |
|-------------|---------|------------|--------------------|-----------|
| groestl_256 | 122K    | 300        | 12.8189            | 31        |

vision_mark_32 demo application coming soon!

### Hash Core Stats
Frequency achievable when instantiating the cores without data movers.
Power is measured just for the hash core.

|                | LUTs | Freq (MHz) | Throughput (GiB/s) | Power (W) |
|----------------|------|------------|--------------------|-----------|
| groestl_256    | 115K | 750        | 44.7               | 12        |
| vision_mark_32 | 374K | 500        | 29.8               | 81        |

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
