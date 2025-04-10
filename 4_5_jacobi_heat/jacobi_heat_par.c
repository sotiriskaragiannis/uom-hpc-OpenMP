// Compile: gcc -O3 jacobi_heat_par.c -o jacobi_heat_par.out -lm -fopenmp
// Execute: ./jacobi_heat_par.out <num_threads>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define maxsize 700
#define iterations 1000000
#define row 20
#define col 20
#define start 100
#define accuracy 32

int main(int argc, char *argv[]) {
  int i, j, k;
  double table1[maxsize][maxsize], table2[maxsize][maxsize];
  double diff;
  double t_start, t_end;

  if (argc != 2) {
		printf ("Usage : %s <num_threads>\n", argv[0]);
		return 1;
  }

  int num_threads = strtol(argv[1], NULL, 10);

  // Set OpenMP threads dynamically
  omp_set_num_threads(num_threads);

  /* Initialize both tables */
  for (i = 0; i < maxsize; i++)
    for (j = 0; j < maxsize; j++) {
      table1[i][j] = 0;
      table2[i][j] = 0;
    }

  t_start = omp_get_wtime();

  /* Repeat for each iteration */
  for (k = 0; k < iterations; k++) {
    /* Create a heat source */
    table1[row][col] = start;

    /* Difference initialization */
    diff = 0.0;

/* Perform the calculations in parallel */
#pragma omp parallel for private(j) reduction(+ : diff)
    for (i = 1; i < maxsize - 1; i++) {
      for (j = 1; j < maxsize - 1; j++) {
        table2[i][j] = 0.25 * (table1[i - 1][j] + table1[i + 1][j] +
                               table1[i][j - 1] + table1[i][j + 1]);
        diff += (table2[i][j] - table1[i][j]) * (table2[i][j] - table1[i][j]);
      }
    }

    /* Print result */
    // for (i = 0; i < maxsize; i++) {
    //   for (j = 0; j < maxsize; j++)
    //     printf("%5.0f ", table2[i][j]);
    //   printf("\n");
    // }
    // printf("\n");

    /* Print difference and check convergence */
    diff = sqrt(diff);
    // printf("diff = %3.25f\n\n", diff);

    if (diff < accuracy) {
      printf("\n\nConvergence in %d iterations\n\n", k);
      break;
    }

/* Copy new table to old table */
#pragma omp parallel for private(j)
    for (i = 0; i < maxsize; i++)
      for (j = 0; j < maxsize; j++)
        table1[i][j] = table2[i][j];
  }

  t_end = omp_get_wtime();
  printf("Calculated in %f seconds\n", t_end - t_start);

  return 0;
}
