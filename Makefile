all:
	mkdir -p build
	cd build;cmake3 ..;make install
#	cd build;./preprocessor youtube
#	cd build;./runner
	cd build;./runner
#	cd build;./runner
#	cd build; sudo perf stat ./runner
	cd build; sudo perf stat -e cache-misses,cache-references,branches,branch-misses,instructions ./runner
	cd build; sudo perf stat -e cache-misses,cache-references,branches,branch-misses,instructions ./runner
#	cd build;./test_driver