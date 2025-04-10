// gcc -O3 backsub_par.c -o backsub_par.out -fopenmp
// ./backsub_par.out <matrix_size> <num_threads>
// Max size 50000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {

  int i, j, N;
  double *x, *b, **a, sum;
  char any;
  double start, end;

  if (argc != 3) {
    printf("Usage : %s <matrix size> <num_threads>\n", argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  int num_threads = strtol(argv[2], NULL, 10);

  // Set OpenMP threads dynamically
  omp_set_num_threads(num_threads);

  /* Allocate space for matrices */
  a = (double **)malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
    a[i] = (double *)malloc((i + 1) * sizeof(double));
  b = (double *)malloc(N * sizeof(double));
  x = (double *)malloc(N * sizeof(double));

  /* Create doubles between 0 and 1. Diagonal elents between 2 and 3. */
  srand(time(NULL));
  for (i = 0; i < N; i++) {
    x[i] = 0.0;
    b[i] = (double)rand() / (RAND_MAX * 2.0 - 1.0);
    a[i][i] = 2.0 + (double)rand() / (RAND_MAX * 2.0 - 1.0);
    for (j = 0; j < i; j++)
      a[i][j] = (double)rand() / (RAND_MAX * 2.0 - 1.0);
    ;
  }

  start = omp_get_wtime();

  /* Calulation */
  #pragma omp parallel for private(sum, j) shared(a, x, b)
  for (i = 0; i < N; i++) {
    sum = 0.0;
    for (j = 0; j < i; j++) {
      sum = sum + (x[j] * a[i][j]);
      // printf ("%d %d %f %f %f \t \n", i, j, x[j], a[i][j], sum);
    }
    x[i] = (b[i] - sum) / a[i][i];
    // printf ("%d %f %f %f %f \n", i, b[i], sum, a[i][i], x[i]);
  }

  end = omp_get_wtime();

  // /* Print result */
  // for (i = 0; i < N; i++) {
  //   for (j = 0; j <= i; j++)
  //     printf("%f \t", a[i][j]);
  //   printf("%f \t%f\n", x[i], b[i]);
  // }

  // /* Check result */
  // for (i = 0; i < N; i++) {
  //   sum = 0.0;
  //   for (j = 0; j <= i; j++)
  //     sum = sum + (x[j] * a[i][j]);
  //   if (fabsf(sum - b[i]) > 0.00001) {
  //     printf("%f != %f\n", sum, b[i]);
  //     printf("Validation Failed...\n");
  //   }
  // }

  for (i = 0; i < N; i++)
    free(a[i]);
  free(a);
  free(b);
  free(x);

  printf("Calculated in %f seconds\n", end - start);
  return 0;
}
