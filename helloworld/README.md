Setup:
   - set LLVM_ROOT and EMSCRIPTEN_ROOT in Makefile

To run hello world in a browser:
   - make hw.html
   - python -m http.server
   - open a browser and connect to host (eg. http://miranda.cs.illinois.edu:8000/)
   - click on the html to run the program

To run with wasmtime (https://wasmtime.dev/):
   - make clean
   - make hw.wasm
   - wasmtime hw.wasm

Switching to the LLVM13 backend produces errors both in wasm standalone mode and html mode. In wasm standalone mode, the build fails with:

- wasm-ld: error: unknown argument: --table-base=1
- wasm-ld: error: unknown file type: /tmp/tmpejo2ohallibemscripten_js_symbols.so
