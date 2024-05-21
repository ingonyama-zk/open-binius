DEVICE_PART ?= xcu250-figd2104-2l-e

csynth:
	v++ -c --mode hls --part $(DEVICE_PART) --config ./hls_config.cfg --work_dir ./work

csim:
	vitis-run --mode hls --csim --config ./hls_config.cfg --work_dir ./work

cosim:
	vitis-run --mode hls --cosim --config ./hls_config.cfg --work_dir ./work

impl:
	vitis-run --mode hls --impl --config ./hls_config.cfg --work_dir ./work

clean:
	rm -rf ./work
