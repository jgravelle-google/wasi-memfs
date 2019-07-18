CC=/s/wbin/clang
LD=/s/wbin/wasm-ld
DIS=/s/wbin/wasm-dis
SYSROOT=/usr/local/google/home/jgravelle/code/wasm/wasi-libc/sysroot
WASI_C_FLAGS=--target=wasm32-unknown-wasi --sysroot=$(SYSROOT)
OPT=-O2
LIBC_A=$(SYSROOT)/lib/wasm32-wasi/libc.a

default: run

clean:
	rm -f memfs.wasm hello.wasm
.PHONY: clean

memfs.wasm:
	$(CC) memfs.c $(WASI_C_FLAGS) -c -o memfs.o $(OPT)
	$(CC) memfs.c $(WASI_C_FLAGS) -S -emit-llvm -o memfs.ll $(OPT)
	$(LD) memfs.o $(LIBC_A) -o memfs.wasm --no-entry --import-memory --allow-undefined
	$(DIS) memfs.wasm -o memfs.wast

hello.wasm: hello.c
	$(CC) hello.c $(WASI_C_FLAGS) -c -o hello.o $(OPT)
	$(LD) hello.o $(LIBC_A) -o hello.wasm --entry main --import-memory
	$(DIS) hello.wasm -o hello.wast

run: hello.wasm memfs.wasm
	node --experimental-modules loader.mjs hello.wasm
.PHONY: run
