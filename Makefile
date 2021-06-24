all: build

SRC = $(wildcard *.cpp)

build: $(SRC)
	cd build; emcmake cmake ../
	emmake make -C build

.PHONY: build
