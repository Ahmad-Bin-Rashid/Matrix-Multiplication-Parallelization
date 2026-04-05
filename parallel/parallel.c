#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define DEFAULT_SIZE 512

/*
 * Parallel Matrix Multiplication using MPI Collectives
 *
 * Strategy:
 *   - Process 0 initialises matrices A and B
 *   - MPI_Bcast sends B to all processes
 *   - MPI_Scatter distributes rows of A across processes
 *   - Each process computes its local rows of C = A_local * B
 *   - MPI_Gather collects the result rows into C on process 0
 *
 * Complexity (parallel): O(N^3 / P) where P = number of processes
 */

void fill_random(double *matrix, int N) {
    for (int i = 0; i < N * N; i++)
        matrix[i] = (double)rand() / RAND_MAX;
}

void sequential_multiply(double *A, double *B, double *C, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            double s = 0.0;
            for (int k = 0; k < N; k++)
                s += A[i * N + k] * B[k * N + j];
            C[i * N + j] = s;
        }
}

int main(int argc, char **argv) {
    int rank, size, N = DEFAULT_SIZE;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
            N = atoi(argv[++i]);
    }

    /* N must be divisible by the number of processes */
    if (N % size != 0) {
        if (rank == 0)
            fprintf(stderr, "Error: N (%d) must be divisible by number of processes (%d)\n", N, size);
        MPI_Finalize();
        return 1;
    }

    int rows_per_proc = N / size;

    double *A        = NULL;
    double *B        = (double *)malloc(N * N * sizeof(double));
    double *C        = NULL;
    double *A_local  = (double *)malloc(rows_per_proc * N * sizeof(double));
    double *C_local  = (double *)malloc(rows_per_proc * N * sizeof(double));

    /* --- Root initialises matrices --- */
    if (rank == 0) {
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));
        srand(42);
        fill_random(A, N);
        fill_random(B, N);

        printf("=== Parallel Matrix Multiplication (MPI Collectives) ===\n");
        printf("Matrix size    : %d x %d\n", N, N);
        printf("Processes      : %d\n", size);
        printf("Rows/process   : %d\n\n", rows_per_proc);
    }

    /* Synchronise before timing */
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    /* --- Broadcast B to all processes --- */
    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* --- Scatter rows of A --- */
    MPI_Scatter(A, rows_per_proc * N, MPI_DOUBLE,
                A_local, rows_per_proc * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    /* --- Each process multiplies its local rows --- */
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            double s = 0.0;
            for (int k = 0; k < N; k++)
                s += A_local[i * N + k] * B[k * N + j];
            C_local[i * N + j] = s;
        }
    }

    /* --- Gather result rows into C on root --- */
    MPI_Gather(C_local, rows_per_proc * N, MPI_DOUBLE,
               C, rows_per_proc * N, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();
    double parallel_time = t_end - t_start;

    /* --- Root prints results and verifies --- */
    if (rank == 0) {
        printf("Parallel time  : %.6f seconds\n", parallel_time);
        printf("C[0][0] = %.6f  (checksum for verification)\n\n", C[0]);

        /* Verification for small N */
        if (N <= 64) {
            double *C_seq = (double *)malloc(N * N * sizeof(double));
            sequential_multiply(A, B, C_seq, N);
            double max_err = 0.0;
            for (int i = 0; i < N * N; i++) {
                double err = C[i] - C_seq[i];
                if (err < 0) err = -err;
                if (err > max_err) max_err = err;
            }
            printf("Verification   : max error = %.2e %s\n",
                   max_err, max_err < 1e-9 ? "(PASSED)" : "(FAILED)");
            free(C_seq);
        }

        /* Report for performance CSV */
        printf("\nCSV_ROW: %d,%d,%.6f\n", N, size, parallel_time);

        free(A);
        free(C);
    }

    free(B);
    free(A_local);
    free(C_local);

    MPI_Finalize();
    return 0;
}
