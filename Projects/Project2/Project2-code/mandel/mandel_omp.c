#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "math.h"

#include "consts.h"
#include "pngwriter.h"

unsigned long get_time() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

int main(int argc, char **argv) {
    png_data *pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

    double fDeltaX = (MAX_X - MIN_X) / (double)IMAGE_WIDTH;
    double fDeltaY = (MAX_Y - MIN_Y) / (double)IMAGE_HEIGHT;
    long nTotalIterationsCount = 0;
    unsigned long nTimeStart = get_time();

    // do the calculation
#pragma omp parallel for collapse(2) schedule(dynamic)
    for (long j = 0; j < IMAGE_HEIGHT; j++) {
        for (long i = 0; i < IMAGE_WIDTH; i++) {
            double x, y, x2, y2, cx, cy;
            cy = MIN_Y + fDeltaY * j;
            cx = MIN_X + fDeltaX * i;
            x = cx;
            y = cy;
            x2 = x * x;
            y2 = y * y;
            // compute the orbit z, f(z), f^2(z), f^3(z), ...
            // count the iterations until the orbit leaves the circle |z|=2.
            // stop if the number of iterations exceeds the bound MAX_ITERS.
            // >>>>>>>> CODE IS MISSING
            long n = 0;
            while (n < MAX_ITERS && x2+y2 < 4){
                y = 2 * x * y + cy;
                x = x2 - y2 + cx;
                // (2 + 3i)^2 = 4 + 2 * 6i - 9;
                x2 = x*x;
                y2 = y*y;

                n += 1;
            }
#pragma omp atomic
            nTotalIterationsCount += n;
            // <<<<<<<< CODE IS MISSING
            // n indicates if the point belongs to the mandelbrot set
            // plot the number of iterations at point (i, j)
            int c = ((long)n * 255) / MAX_ITERS;
            png_plot(pPng, i, j, c, c, c);
        }
    }
    unsigned long nTimeEnd = get_time();

    // print benchmark data
    printf("Total time:                 %g millisconds\n",
           (nTimeEnd - nTimeStart) / 1000.0);
    printf("Image size:                 %ld x %ld = %ld Pixels\n",
           (long)IMAGE_WIDTH, (long)IMAGE_HEIGHT,
           (long)(IMAGE_WIDTH * IMAGE_HEIGHT));
    printf("Total number of iterations: %ld\n", nTotalIterationsCount);
    printf("Avg. time per pixel:        %g microseconds\n",
           (nTimeEnd - nTimeStart) / (double)(IMAGE_WIDTH * IMAGE_HEIGHT));
    printf("Avg. time per iteration:    %g microseconds\n",
           (nTimeEnd - nTimeStart) / (double)nTotalIterationsCount);
    printf("Iterations/second:          %g\n",
           nTotalIterationsCount / (double)(nTimeEnd - nTimeStart) * 1e6);
    // assume there are 8 floating point operations per iteration
    printf("MFlop/s:                    %g\n",
           nTotalIterationsCount * 8.0 / (double)(nTimeEnd - nTimeStart));

    png_write(pPng, "mandel.png");
    return 0;
}
