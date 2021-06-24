all: build

SRC = $(wildcard *.cpp)

build: $(SRC)
	mkdir -p build
	cd build; emcmake cmake ../
	emmake make -C build

.PHONY: build
