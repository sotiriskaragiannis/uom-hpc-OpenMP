// Compile: gcc -O3 count_sort_par.c -o count_sort_par.out -fopenmp
// Execute: ./count_sort_par.out <array_size>
// Notes: max size 200000
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define UPPER 1000
#define LOWER 0

int main(int argc, char *argv[]) {
  int *x, *y;
  int i, j, my_num, my_place;
  double start, end;

  if (argc != 3) {  // Expecting 2 arguments now
    printf("Usage: %s <array_size> <num_threads>\n", argv[0]);
    return 1;
  }

  int n = strtol(argv[1], NULL, 10);
  int num_threads = strtol(argv[2], NULL, 10);

  // Set OpenMP threads dynamically
  omp_set_num_threads(num_threads);

  x = (int *)malloc(n * sizeof(int));
  y = (int *)malloc(n * sizeof(int));

  // Initialize array
  for (i = 0; i < n; i++)
    x[i] = n - i;

  start = omp_get_wtime();

  #pragma omp parallel for private(my_num, my_place, i) shared(n, x, y)
  for (j = 0; j < n; j++) {
    my_num = x[j];
    my_place = 0;
    for (i = 0; i < n; i++)
      if ((my_num > x[i]) || ((my_num == x[i]) && (j < i)))
        my_place++;
    y[my_place] = my_num;
  }

  end = omp_get_wtime();

  // for (i = 0; i < n; i++)
  //   printf("%d\n", y[i]);
  printf("Done in %f seconds\n", end - start);

  free(x);
  free(y);

  return 0;
}
