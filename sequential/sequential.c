#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DEFAULT_SIZE 512

/*
 * Sequential Matrix Multiplication
 * Computes C = A * B for N x N matrices
 * Original complexity: O(N^3)
 */

void matrix_multiply(double *A, double *B, double *C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void fill_random(double *matrix, int N) {
    for (int i = 0; i < N * N; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

void print_matrix(double *M, int N, const char *name) {
    printf("Matrix %s (%dx%d):\n", name, N, N);
    for (int i = 0; i < N && i < 6; i++) {
        for (int j = 0; j < N && j < 6; j++) {
            printf("%8.4f ", M[i * N + j]);
        }
        if (N > 6) printf("...");
        printf("\n");
    }
    if (N > 6) printf("...\n");
    printf("\n");
}

int main(int argc, char **argv) {
    int N = DEFAULT_SIZE;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            N = atoi(argv[++i]);
        }
    }

    printf("=== Sequential Matrix Multiplication ===\n");
    printf("Matrix size: %d x %d\n\n", N, N);

    srand(42);

    double *A = (double *)malloc(N * N * sizeof(double));
    double *B = (double *)malloc(N * N * sizeof(double));
    double *C = (double *)malloc(N * N * sizeof(double));

    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    fill_random(A, N);
    fill_random(B, N);

    if (N <= 8) {
        print_matrix(A, N, "A");
        print_matrix(B, N, "B");
    }

    /* Time the computation */
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    matrix_multiply(A, B, C, N);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    if (N <= 8) {
        print_matrix(C, N, "C (result)");
    }

    printf("Sequential time: %.6f seconds\n", elapsed);
    printf("C[0][0] = %.6f  (checksum for verification)\n", C[0]);

    free(A);
    free(B);
    free(C);
    return 0;
}
