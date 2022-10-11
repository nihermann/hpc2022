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

const int min(const int a, const int b)
{
    return a < b? a : b;
}


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */    
void square_dgemm (const int n, double* A, double* B, double* C, const int block_size)
{
    // TODO: Implement the blocking optimization

    double* AT = (double*) malloc(n*n*sizeof(double));
    for (int i = 0; i < n; ++i) {
        const int in = n*i;
        for (int j = 0; j < n; ++j) {
            AT[i+j*n] = A[j+in];
        }
    }


    for (int ii = 0; ii <= n; ii += block_size) {
        for (int jj = 0; jj <= n; jj += block_size) {
            for (int kk = 0; kk <= n; kk += block_size) {

                const int i_limit = min(ii + block_size, n);
                for (int i = ii; i < i_limit; ++i) {
                    const int j_limit = min(jj + block_size, n);
                    for (int j = jj; j < j_limit; ++j) {
                        const int in = i*n;
                        const int jn = j*n;
                        double cij = C[i+jn];
                        const int k_limit = min(kk + block_size, n);
                        for (int k = kk; k < k_limit; ++k) {
                            cij += AT[k+in] * B[k+jn];
                        }
                        C[i+jn] = cij;
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