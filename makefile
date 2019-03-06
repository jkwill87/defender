# MacOS Mojave
LIBS = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm

## Older Versions
# LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

FLAGS = -O2 -pedantic -std=c99 -Wall -Wno-deprecated-declarations

a2:
	clang ${FLAGS} -Iinclude src/exec/*.c src/graphics/*.c src/world/*.c -o a2 ${LIBS}
	ln -fs a2 a1

clean:
	rm -rfv build a1 a2

.PHONY: a2 clean

