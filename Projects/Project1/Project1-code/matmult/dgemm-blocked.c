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
#include "stdio.h"

const char* dgemm_desc = "Naive, three-loop dgemm.";

int min(int a, int b)
{
    return a < b? a : b;
}

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */    
void square_dgemm (int n, double* A, double* B, double* C, int b)
{
    // TODO: Implement the blocking optimization
    int block_size = b;
    int num_blocks = n / block_size;

    for (int ii = 1; ii <= num_blocks + 1; ++ii) {
        for (int jj = 1; jj <= num_blocks + 1; ++jj) {
            for (int kk = 1; kk <= num_blocks + 1; ++kk) {
                for (int i = (ii - 1) * block_size; i < min(ii * block_size, n); ++i) {
                    for (int j = (jj - 1) * block_size; j < min(jj * block_size, n); ++j) {
                        double cij = C[i+j*n];
                        for (int k = (kk - 1) * block_size; k < min(kk * block_size, n); ++k) {
                            cij += A[i+k*n] * B[k+j*n];
                        }
                        C[i+j*n] = cij;
                    }
                }
            }
        }
    }
}



//int main(int argc, char **argv)
//{
//    double A[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25.};
//    double B[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25.};
//    double C[] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
//    double Cstar[] = {215.,  230.,  245.,  260.,  275.,
//                     490.,  530.,  570.,  610.,  650.,
//                     765.,  830.,  895.,  960., 1025.,
//                     1040., 1130., 1220., 1310., 1400.,
//                     1315., 1430., 1545., 1660., 1775.};
//
//    square_dgemm(5, A, B, C);
//
//    int same = 0;
//    for (int i = 0; i < 25; ++i) {
//        printf("is %f should be %f\n", C[i], Cstar[i]);
//        same += Cstar[i] == C[i];
//    }
//    printf("%d correct", same);
//
//
//}