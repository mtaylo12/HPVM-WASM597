SRC = matmul.cpp
LLVM_BUILD_DIR = ../../hpvm/hpvm/build/
CUDA_TOOLKIT_PATH = /software/cuda-toolkit-8.0
HPVM_ROOT=../../hpvm

# compile executable (for CPU)
cpu-build: matmul-extract-cpu.linked.ll   
	$(LLVM_BUILD_DIR)/bin/clang++ -O3 -lm -lpthread -lrt -lOpenCL -L$(CUDA_TOOLKIT_PATH)/lib64 --target=wasm64 $< -o matmul_seq.ll

# link in hpvm runtime
matmul-extract-cpu.linked.ll: matmul-extract-cpu.hpvm.host.ll
	$(LLVM_BUILD_DIR)/bin/llvm-link $< $(LLVM_BUILD_DIR)/tools/hpvm/projects/hpvm-rt/hpvm-rt.bc -S -o $@

# optimization step - memory graph constructed explicitly, produces host LLVM IR from HPVM-C
matmul-extract-cpu.hpvm.host.ll: matmul-extract.bc
	$(LLVM_BUILD_DIR)/bin/opt -enable-new-pm=0 -load $(LLVM_BUILD_DIR)/lib/HPVMIRCodeGen.so -load $(LLVM_BUILD_DIR)/lib/HPVMTargets.so -genhpvm -globaldce -hpvm-timers-gen -dfg2llvm-cpu -clearDFG -hpvm-timers-cpu -hpvm-timers-ptx -S $< -o $@

# hcc is a compiler developed by AMD for heterogeneous environments, output describes HPVM DFG in HPVM-C
matmul-extract.bc: matmul.bc
	$(LLVM_BUILD_DIR)/bin/hcc -declsfile  $(LLVM_BUILD_DIR)/tools/hpvm/projects/hetero-c++/lib//HPVMCFunctionDeclarations/HPVMCFunctionDeclarations.bc -o matmul-extract.bc $<

# take LLVM IR to bitcode (which can be further processed)
matmul.bc: matmul.ll
	$(LLVM_BUILD_DIR)/bin/llvm-as $<

# use clang to compile to LLVM IR - modified for wasm64 target
matmul.ll: $(SRC)
	$(LLVM_BUILD_DIR)/bin/clang --target=wasm32 -fno-discard-value-names -I/usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0 -I /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/x86_64-linux-gnu/c++/7.5.0 -I /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/backward -I /home/mayat4/hpvm/hpvm/build/lib/clang/13.0.0/include -I  /usr/local/include -I  /usr/include/x86_64-linux-gnu -I  /usr/include -O1 -S -emit-llvm -I$(HPVM_ROOT)/hpvm/projects/hetero-c++/test/include/ $< -o matmul.ll	

#original matmul.ll compilation step:
#$(LLVM_BUILD_DIR)/bin/clang -fno-discard-value-names  -O1 -S -emit-llvm  -I../hpvm/hpvm/projects/hetero-c++/test/include/ $< -o matmul.ll

clean:
	rm *.bc *.ll # matmul_seq
