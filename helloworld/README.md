Setup:
   - set LLVM_ROOT and EMSCRIPTEN_ROOT in Makefile

To run hello world in a browser:
   - make hw.html
   - python -m http.server
   - open a browser and connect to host (eg. http://miranda.cs.illinois.edu:8000/)
   - click on the html to run the program

To run with wasmtime:
   - make clean
   - make hw.wasm
   - wasmtime hw.wasm
