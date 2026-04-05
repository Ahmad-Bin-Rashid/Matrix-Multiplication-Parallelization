# ══════════════════════════════════════════════════════════════════
# Makefile — Parallel Matrix Multiplication (MPI Collectives)
# Usage:
#   make              → build the binary
#   make run NP=4     → run with 4 processes, default N=512
#   make run NP=4 N=1024 → run with 4 processes and N=1024
#   make verify       → correctness check (N=32, 4 processes)
#   make bench        → run benchmarks for NP=1,2,4,8 with N=512
#   make clean        → remove binary
# ══════════════════════════════════════════════════════════════════

MPICC   = mpicc
CFLAGS  = -O2 -Wall -Wextra
TARGET  = parallel
SRC     = parallel.c
N       = 512
NP      = 4

.PHONY: all run verify bench clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(MPICC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Build successful: ./$(TARGET)"

run: $(TARGET)
	@if [ $(NP) -eq 8 ]; then \
		mpirun --oversubscribe -np $(NP) ./$(TARGET) -n $(N); \
	else \
		mpirun -np $(NP) ./$(TARGET) -n $(N); \
	fi
	

verify: $(TARGET)
	@echo "--- Correctness check: N=32, NP=4 ---"
	mpirun -np 4 ./$(TARGET) -n 32

bench: $(TARGET)
	@echo "--- Benchmark: N=$(N) ---"
	@for np in 1 2 4 8; do \
		echo "  NP=$$np:"; \
		if [ $$np -eq 8 ]; then \
			mpirun --oversubscribe -np $$np ./$(TARGET) -n $(N); \
		else \
			mpirun -np $$np ./$(TARGET) -n $(N); \
		fi \
	done

clean:
	rm -f $(TARGET)
	@echo "Cleaned."
