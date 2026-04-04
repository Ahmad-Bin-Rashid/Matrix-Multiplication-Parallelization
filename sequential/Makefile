# ══════════════════════════════════════════════════════════════════
# Makefile — Sequential Matrix Multiplication
# Usage:
#   make            → build the binary
#   make run        → build and run with default N=512
#   make run N=1024 → build and run with custom size
#   make clean      → remove binary
# ══════════════════════════════════════════════════════════════════

CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -lm
TARGET  = sequential
SRC     = sequential.c
N       = 512

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Build successful: ./$(TARGET)"

run: $(TARGET)
	./$(TARGET) -n $(N)

clean:
	rm -f $(TARGET)
	@echo "Cleaned."
