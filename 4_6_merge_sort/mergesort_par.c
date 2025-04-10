// Compile: gcc -O3 mergesort_par.c -o mergesort_par.out -fopenmp
// Execute: ./mergesort_par.out <list_size> <num_threads>
// Max size: 100000000
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MIN_BLOCK 10000

void generate_list(int *x, int n) {
    int i;
    srand(time(NULL));
    for (i = 0; i < n; i++)
        x[i] = rand() % n;
}

void merge(int *X, int n, int *tmp) {
    int i = 0, j = n / 2, ti = 0;
    while (i < n / 2 && j < n) {
        if (X[i] < X[j]) {
            tmp[ti++] = X[i++];
        } else {
            tmp[ti++] = X[j++];
        }
    }
    while (i < n / 2) tmp[ti++] = X[i++];
    while (j < n) tmp[ti++] = X[j++];
    memcpy(X, tmp, n * sizeof(int));
}

void mergesort(int *X, int n, int *tmp) {
    if (n < 2) return;

    if (n < MIN_BLOCK) { // Sequential execution for small blocks
        mergesort(X, n / 2, tmp);
        mergesort(X + (n / 2), n - (n / 2), tmp);
        merge(X, n, tmp);
        return;
    }

    #pragma omp task shared(X, tmp)
    mergesort(X, n / 2, tmp);

    #pragma omp task shared(X, tmp)
    mergesort(X + (n / 2), n - (n / 2), tmp);

    #pragma omp taskwait
    merge(X, n, tmp);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <list size> <num_threads>\n", argv[0]);
        return 1;
    }

    int n = strtol(argv[1], NULL, 10);
    int num_threads = strtol(argv[2], NULL, 10);

    // Set OpenMP threads dynamically
    omp_set_num_threads(num_threads);

    int *data = (int *)malloc(sizeof(int) * n);
    int *tmp = (int *)malloc(sizeof(int) * n);

    generate_list(data, n);

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        mergesort(data, n, tmp);
    }

    double end = omp_get_wtime();
    printf("Sorted in %f seconds\n", end - start);

    free(data);
    free(tmp);
    return 0;
}
