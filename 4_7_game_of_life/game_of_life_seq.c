// HPC course University of Macedonia
// Compile: gcc -O3 game_of_life_seq.c -o game_of_life_seq.out -fopenmp
// Execute: ./game_of_life_seq.out
#include <stdio.h>
#include <omp.h>

#define M 100
#define N 100

void printGrid(int mat[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == 1) {
                printf("1 ");  // Print 1 as a distinct character (can be * as well)
            } else {
                printf("- ");  // Print a space for the "0" to simulate white background
            }
        }
        printf("\n");
    }
}

void findNextGen(int mat[M][N]) {
    int nextGen[M][N];
    int directions[8][2] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int live = 0;
            for (int k = 0; k < 8; k++) {
                int x = i + directions[k][0];
                int y = j + directions[k][1];
                if (x >= 0 && x < M && y >= 0 && y < N && mat[x][y] == 1) {
                    live++;
                }
            }

            if (mat[i][j] == 1 && (live < 2 || live > 3)) {
                nextGen[i][j] = 0;
            } else if (mat[i][j] == 0 && live == 3) {
                nextGen[i][j] = 1;
            } else {
                nextGen[i][j] = mat[i][j];
            }
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = nextGen[i][j];
        }
    }
}

void simulateGenerations(int mat[M][N], int generations) {
    for (int gen = 0; gen < generations; gen++) {
        findNextGen(mat);
    }

    // printf("\nFinal state after %d generations:\n", generations);
    // printGrid(mat);
}

int main() {
    int mat[M][N] = {0};

    // initial state
    int gx = 20, gy = 20;
    mat[gx][gy + 1] = mat[gx][gy + 2] = 1;
    mat[gx + 1][gy] = mat[gx + 1][gy + 3] = 1;
    mat[gx + 2][gy - 1] = mat[gx + 2][gy + 4] = 1;
    mat[gx + 3][gy - 1] = mat[gx + 3][gy + 4] = 1;
    mat[gx + 4][gy + 1] = mat[gx + 4][gy + 2] = 1;
    mat[gx + 5][gy] = mat[gx + 5][gy + 3] = 1;
    mat[gx + 6][gy + 1] = mat[gx + 6][gy + 2] = 1;

    int gx2 = M - gx, gy2 = N - gy;
    for (int dx = 0; dx < 10; dx++)
        for (int dy = 0; dy < 10; dy++)
            mat[gx2 + dx][gy2 + dy] = mat[gx + dx][gy + dy];

    // printf("Initial state:\n");
    // printGrid(mat);

    int generations = 100000;
    double start = omp_get_wtime();

    simulateGenerations(mat, generations);

    double end = omp_get_wtime();
    printf("Calculated in %f seconds\n", end - start);

    return 0;
}
