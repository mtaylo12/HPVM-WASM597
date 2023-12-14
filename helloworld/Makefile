EMSCRIPTEN_ROOT=../../emscripten
LLVM_ROOT=../../llvm/llvm-project

hw.html: hw.ll
	$(EMSCRIPTEN_ROOT)/emcc $< -o $@

hw.wasm: hw.ll
	$(EMSCRIPTEN_ROOT)/emcc $< -o $@

hw.ll: hw.cpp
	$(LLVM_ROOT)/build/bin/clang++ --target=wasm32-unknown-emscripten --sysroot=$(EMSCRIPTEN_ROOT)/cache/sysroot -S -emit-llvm $< -c -o $@

clean:
	rm -rf hw.ll hw.wasm hw.html hw.js
