CC=clang
C_FLAGS=--target=riscv64-linux-gnu -isystem /usr/riscv64-linux-gnu/include

harness: *.c
	$(CC) $(C_FLAGS) -o $@ $^

run_qemu: harness
	qemu-riscv64 -L /usr/riscv64-linux-gnu harness