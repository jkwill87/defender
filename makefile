a3:
	mkdir -p build
	cd build && cmake .. && make && cp a3 ..
	ln -s a3 a1

clean:
	rm -rfv *build* a1 a3

.PHONY: a3 clean

