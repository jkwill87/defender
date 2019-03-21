libs_dir := -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
c99_source := $(wildcard src/*/*.c)
cpp_source := $(wildcard src/*/*.cpp)
flags := -Wno-deprecated-declarations -O3
libs := -framework OpenGL -framework GLUT -lm

# works on MacOS Mojave
a3:
	clang -std=c99 -Iinclude ${flags} ${libs_dir} -c ${c99_source}
	clang++ -std=c++11 -Iinclude ${flags} ${libs_dir} -c ${cpp_source}
	clang++ -g -o a3 *.o ${libs}
	ln -sf a3 a1


clean:
	rm -rfv *build* a1 a3 *.o

.PHONY: a3 clean
