PART=xcu250-figd2104-2l-e
KERNEL=vision_mark_32

csim:
	mkdir -p work
	cd work; vitis-run --csim --config ../$(KERNEL).cfg --work_dir ./$(KERNEL)

csynth:
	mkdir -p work
	cd work; v++ -c --mode hls --part $(PART) --config ../$(KERNEL).cfg

cosim:
	cd work; vitis-run --cosim --config ../$(KERNEL).cfg --work_dir ./$(KERNEL)

impl:
	cd work; vitis-run --impl --config ../$(KERNEL).cfg --work_dir ./$(KERNEL)

clean:
	rm -rf ./work