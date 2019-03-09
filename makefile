build_dir := build
libs_dir := -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
c99_source := $(addprefix ../, $(wildcard src/*/*.c))
cpp_source := $(addprefix ../, $(wildcard src/*/*.cpp))
flags := -Wall -Wno-deprecated-declarations -O3
libs := -framework OpenGL -framework GLUT -lm

# works on MacOS Mojave
a3:
	mkdir -p ${build_dir}
	cd ${build_dir} && clang -std=c99 -I../include ${flags} ${libs_dir} -c ${c99_source}
	cd ${build_dir} && clang++ -std=c++11 -I../include ${flags} ${libs_dir} -c ${cpp_source}
	cd ${build_dir} && clang++ -g -o a3 *.o ${libs}
	ln -sf ${build_dir}/a3 a3
	ln -sf ${build_dir}/a3 a1

# works elsewhere cmake is installed
a3-omp:
	mkdir -p ${build_dir}
	cd build && cmake .. && make
	ln -sf ${build_dir}/a3 a3
	ln -sf ${build_dir}/a3 a1

clean:
	rm -rfv *build* a1 a3

.PHONY: a3 clean

