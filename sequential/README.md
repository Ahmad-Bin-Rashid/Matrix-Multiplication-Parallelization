# Sequential Matrix Multiplication

## Problem Description
Standard O(N³) matrix multiplication: C = A × B for N×N matrices.  
Implemented in C as the baseline for performance comparison.

## Compilation
```bash
gcc -O2 -o sequential sequential.c -lm
```

## Example Usage

### Without Makefile
```bash
./sequential -n 256    # 256×256 matrices
./sequential -n 512    # 512×512 matrices
./sequential -n 1024   # 1024×1024 matrices
```

### With Makefile
```bash
make                # build the binary
make run            # build and run with default N=512
make run N=1024     # build and run with custom size
make clean          # remove binary
```

## Example Output
```
=== Sequential Matrix Multiplication ===
Matrix size: 1024 x 1024

Sequential time: 3.712000 seconds
C[0][0] = 256.342187  (checksum for verification)
```

## Complexity Analysis
- **Time:** O(N³) — three nested loops each of length N
- **Space:** O(N²) — three N×N matrices in memory
- **Bottleneck:** The innermost dot-product loop runs N³ times
