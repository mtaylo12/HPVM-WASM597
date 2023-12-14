EMSDK_ROOT=/home/mayat4/emsdk
HPVM_ROOT=/home/mayat4/hpvm/hpvm
EMCC_FROM_SOURCE=/home/mayat4/emscripten/emcc

hw-emcc.wasm: hw-emcc.ll
	$(EMCC_FROM_SOURCE) hw.ll -o hw-emcc.wasm

hw-emcc.ll: hw.cpp
	$(EMSDK_ROOT)/upstream/bin/clang++ --target=wasm32-unknown-emscripten --sysroot=/home/mayat4/emsdk/upstream/emscripten/cache/sysroot -S -emit-llvm -v hw.cpp -c -o hw-emcc.ll

clean:
	rm -rf hw-emcc.ll hw-emcc.wasm
