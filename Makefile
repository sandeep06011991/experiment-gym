all:
	mkdir -p build
	cd build;cmake3 ..;make install
#	cd build;./preprocessor youtube
	cd build;./runner
