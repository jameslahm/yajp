all: gen build

SRC = $(wildcard *.cpp)
EMSDK = /home/wangao/projects/emsdk

gen: $(SRC)
	mkdir -p build
	cmake -S . -B build \
		"-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$(EMSDK)/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
		"-DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake" \
		"-DVCPKG_TARGET_TRIPLET=wasm32-emscripten" \
		"-DCMAKE_EXPORT_COMPILE_COMMANDS=1" \
		"-DCMAKE_CXX_FLAGS=-I$(EMSDK)/upstream/emscripten/cache/sysroot/include"

build: $(SRC)
	cmake --build build

.PHONY: gen build
