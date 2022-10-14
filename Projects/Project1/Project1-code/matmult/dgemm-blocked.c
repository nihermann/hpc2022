/* 
    Please include compiler name below (you may also include any other modules you would like to be loaded)

COMPILER= gnu

    Please include All compiler flags and libraries as you want them run. You can simply copy this over from the Makefile's first few lines
 
CC = cc
OPT = -O3
CFLAGS = -Wall -std=gnu99 $(OPT)
MKLROOT = /opt/intel/composer_xe_2013.1.117/mkl
LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm

*/
#include <stdlib.h>

const char* dgemm_desc = "Naive, three-loop dgemm.";

/**
 * Returns the min value of a and b.
 * @param a
 * @param b
 * @return min of (a, b)
 */
int min(int a,  int b)
{
    return a < b? a : b;
}

/**
 * Transposes square matrix A.
 * @param n dim of the matrix.
 * @param A Matrix to transpose.
 * @param AT Resulting matrix.
 */
void transpose(int n, double* A, double* AT){
    for (int i = 0; i < n; ++i) {
        int in = n*i;
        for (int j = 0; j < n; ++j) {
            AT[i+j*n] = A[j+in];
        }
    }
}

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm(int n, double* A, double* B, double* C, int block_size)
{
    // allocate a contiguous block of memory for the transposed matrix A
    double* AT = (double*) malloc(n*n*sizeof(double));
    transpose(n, A, AT); // transpose A for spatial locality

    // Iterate through the blocks
    for (int offset_i = 0; offset_i <= n; offset_i += block_size) {
        for (int offset_j = 0; offset_j <= n; offset_j += block_size) {
            for (int offset_k = 0; offset_k <= n; offset_k += block_size) {
                // Perform matrix multiplication within the blocks.
                // cache limit of the for loop.
                int i_limit = min(offset_i + block_size, n);
                for (int i = offset_i; i < i_limit; ++i) {
                    int j_limit = min(offset_j + block_size, n);
                    for (int j = offset_j; j < j_limit; ++j) {
                        // cache more computations...
                        int in = i*n;
                        int jn = j*n;
                        double cij = C[i+jn];

                        int k_limit = min(offset_k + block_size, n);
                        for (int k = offset_k; k < k_limit; ++k) {
                            cij += AT[k+in] * B[k+jn];
                        }
                        C[i+jn] = cij;
                    }
                }
            }
        }
    }
}

