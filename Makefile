all: build

SRC = $(wildcard *.cpp)

build: $(SRC)
	cd build; emcmake cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../
	emmake make -C build

.PHONY: build
