// Compile: gcc -O3 histogram_par.c -o histogram_par.out -fopenmp
// Execute: ./histogram_par.out <num_threads>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    double start, end;
    FILE *fIn = fopen("original.bmp", "rb");
    FILE *fOut = fopen("new.bmp", "wb");
    if (!fIn || !fOut)
    {
        printf("File error.\n");
        return 0;
    }

    if (argc != 2) {  // Expecting 1 arguments now
      printf("Usage: %s <num_threads>\n", argv[0]);
      return 1;
    }

    int num_threads = strtol(argv[1], NULL, 10);

    // Set OpenMP threads dynamically
    omp_set_num_threads(num_threads);

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fIn);
    fwrite(header, sizeof(unsigned char), 54, fOut);

    int width = *(int*)&header[18];
    int height = abs(*(int*)&header[22]);
    int stride = (width * 3 + 3) & ~3;
    int padding = stride - width * 3;

    printf("width: %d (%d)\n", width, width * 3);
    printf("height: %d\n", height);
    printf("stride: %d\n", stride);
    printf("padding: %d\n", padding);

    // Read entire image into buffer
    unsigned char *imageData = (unsigned char *)malloc(stride * height);
    fread(imageData, sizeof(unsigned char), stride * height, fIn);
    fclose(fIn);

    int histogram[256][4] = {0}; // Global histogram

    start = omp_get_wtime();

    #pragma omp parallel
    {
        int local_histogram[256][4] = {0}; // Private histogram for each thread

        #pragma omp for
        for (int y = 0; y < height; ++y)
        {
            unsigned char *row = imageData + y * stride;
            for (int x = 0; x < width; ++x)
            {
                unsigned char *pixel = row + x * 3;
                local_histogram[pixel[0]][0]++;
                local_histogram[pixel[1]][1]++;
                local_histogram[pixel[2]][2]++;
                unsigned char gray = pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.114;
                local_histogram[gray][3]++;
                memset(pixel, gray, 3);
            }
        }

        // Merge local histograms into global histogram
        #pragma omp critical
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 4; j++)
                histogram[i][j] += local_histogram[i][j];
    }

    // Write processed image
    fwrite(imageData, sizeof(unsigned char), stride * height, fOut);
    fclose(fOut);
    free(imageData);

    end = omp_get_wtime();
    printf("Calculated in %f seconds\n", end - start);

    return 0;
}
