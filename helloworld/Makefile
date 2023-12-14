EMSDK_ROOT=/home/mayat4/emsdk
HPVM_ROOT=/home/mayat4/hpvm/hpvm
EMCC_FROM_SOURCE=/home/mayat4/emscripten/emcc
WASMTIME=wasmtime

hw.html: hw.ll
	$(EMCC_FROM_SOURCE) $< -o $@

hw.wasm: hw.ll
	$(EMCC_FROM_SOURCE) $< -o $@

hw.ll: hw.cpp
	$(EMSDK_ROOT)/upstream/bin/clang++ --target=wasm32-unknown-emscripten --sysroot=/home/mayat4/emsdk/upstream/emscripten/cache/sysroot -S -emit-llvm $< -c -o $@

clean:
	rm -rf hw.ll hw.wasm hw.html hw.js
