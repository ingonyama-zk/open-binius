XRT_PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1

../../kernels/groestl_256/work/m_axi_groestl_256.xo:
	make -C ../../kernels/groestl_256/ csynth

work/$(XRT_PLATFORM)/m_axi_groestl_256/hw/m_axi_groestl_256.xclbin:
	mkdir -p ./work/$(XRT_PLATFORM)/m_axi_groestl_256/hw/
	cd ./work/$(XRT_PLATFORM)/m_axi_groestl_256/hw/ ; \
	v++ --link --target hw --platform $(XRT_PLATFORM) --save-temps ../../../../../../kernels/groestl_256/work/*.xo -o m_axi_groestl_256.xclbin

work/$(XRT_PLATFORM)/m_axi_groestl_256/hw_emu/hw_emu_m_axi_groestl_256.xclbin:
	mkdir -p ./work/$(XRT_PLATFORM)/m_axi_groestl_256/hw_emu/
	cd ./work/$(XRT_PLATFORM)/m_axi_groestl_256/hw_emu/ ; \
	v++ --link --target hw_emu --platform $(XRT_PLATFORM) --save-temps ../../../../../../kernels/groestl_256/work/*.xo -o hw_emu_m_axi_groestl_256.xclbin

demo:
	g++ -g -std=c++17 -I$(XILINX_XRT)/include -L$(XILINX_XRT)/lib -o demo demo.cpp -lxrt_coreutil -pthread
	./demo

clean:
	rm -rf work
	make -C ../../kernels/groestl_256/ clean