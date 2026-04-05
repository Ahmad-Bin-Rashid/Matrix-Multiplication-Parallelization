# Parallel Matrix Multiplication — MPI Collectives

## Parallelisation Approach
**MPI Collective operations** (Scatter / Broadcast / Gather):

1. **MPI_Bcast** — root sends full matrix B to all processes
2. **MPI_Scatter** — root distributes N/P rows of A to each process
3. Local computation — each process multiplies its rows independently
4. **MPI_Gather** — root collects all result rows into C

## Data Distribution Strategy
- Matrix A is split into horizontal slices of `rows_per_proc = N / P` rows
- Matrix B is replicated on every process via Bcast
- No halo exchange needed — row slices are independent
- N must be divisible by P (enforced with runtime check)

## Compilation
```bash
mpicc -O2 -o parallel parallel.c
```

## Example Usage

### Without Makefile
```bash
mpirun -np 1 ./parallel -n 1024    # baseline (no parallelism)
mpirun -np 2 ./parallel -n 1024
mpirun -np 4 ./parallel -n 1024
mpirun --oversubscribe -np 8 ./parallel -n 1024
```

## With Makefile
```bash
make                    # build the binary
make run NP=4           # run with 4 processes, default N=512
make run NP=4 N=1024    # run with 4 processes and N=1024
make verify             # correctness check (N=32, 4 processes)
make bench              # run benchmarks for NP=1,2,4,8 with N=512
make clean              # remove binary
```

## Example Output
```
=== Parallel Matrix Multiplication (MPI Collectives) ===
Matrix size    : 1024 x 1024
Processes      : 4
Rows/process   : 256

Parallel time  : 0.981000 seconds
C[0][0] = 256.342187  (checksum for verification)

CSV_ROW: 1024,4,0.981000
```

## Verification
For N ≤ 64, the program automatically compares with the sequential result:
```bash
mpirun -np 4 ./parallel -n 32
# Verification   : max error = 1.23e-14 (PASSED)
```
