PART=xcu250-figd2104-2l-e
KERNEL=dummy

csim:
	mkdir -p work/$(KERNEL)
	cd work/$(KERNEL); vitis-run --csim --config ../../$(KERNEL).cfg  --work_dir .

csynth:
	mkdir -p work/$(KERNEL)
	cd work/$(KERNEL); v++ -c --mode hls --part $(PART) --config ../../$(KERNEL).cfg

cosim:
	cd work/$(KERNEL); vitis-run --cosim --config ../../$(KERNEL).cfg --work_dir .

impl:
	cd work/$(KERNEL); vitis-run --impl --config ../../$(KERNEL).cfg --work_dir ./$(KERNEL)

clean:
	rm -rf ./work