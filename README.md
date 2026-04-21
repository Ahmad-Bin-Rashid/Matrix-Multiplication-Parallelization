# Matrix Multiplication Parallelization

This is a parallel computing assignment for the course **Parallel and Distributed Computing**. It involes matrix multiplication of N×N matrices of doubles and using MPI Collectives for parallelization.

## Problem Description

Matrix multiplication (C = A × B) for N×N matrices of doubles.  
Originally implemented sequentially in Linear Algebra (3rd semester).  
Parallelised using **MPI Collectives**: `MPI_Bcast`, `MPI_Scatter`, `MPI_Gather`.

## How to Run

### Sequential Version

```bash
gcc -O2 -o sequential/sequential sequential/sequential.c -lm
./sequential/sequential -n 1024
```

### Parallel Version

```bash
mpicc -O2 -o parallel/parallel parallel/parallel.c
mpirun -np 1 ./parallel/parallel -n 1024
mpirun -np 2 ./parallel/parallel -n 1024
mpirun -np 4 ./parallel/parallel -n 1024
mpirun --oversubscribe -np 8 ./parallel/parallel -n 1024
```

### Using Makefile

Commands given in README of respective folders

## Results Summary

| N   | Processes | Speedup | Efficiency |
| --- | --------- | ------- | ---------- |
| 512 | 2         | 1.45×   | 73%        |
| 512 | 4         | 2.25×   | 56%        |
| 512 | 8         | 2.82×   | 35%        |

- **Best speedup achieved:** 2.82× with 8 processes (N=512)
- **Efficiency at highest process count:** 35% (N=512, P=8)

## Repository Structure

```
parallel-assignment-[roll-number]/
├── sequential/      ← original sequential implementation
├── parallel/        ← MPI Collectives parallel implementation
├── results/         ← timing data (CSV) and performance plots (PNG)
├── report/          ← final report PDF
└── README.md
```

## Course Details

- **Course:** Parallel and Distributed Computing
- **Instructor:** Waqas Ali
- **Semester:** 6th
