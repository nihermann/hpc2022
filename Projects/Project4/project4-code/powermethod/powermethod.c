/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School.    *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: : Parallel matrix-vector multiplication and the     *
 *            and power method                                  *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/


#include <stdio.h>
#include <mpi.h>
#include "math.h"
#include "stdlib.h"
#include "hpc-power.h"

double norm(double* vec, int n){
    double sprod = 0;
    for (int i = 0; i < n; ++i) {
        sprod += vec[i]*vec[i];
    }
    return sqrt(sprod);
}

void normalize(double* vec, int n){
    double len = 1/norm(vec, n);
    for (int i = 0; i < n; ++i) {
        vec[i] *= len;
    }
}

double* matVec(double* A, double* vec, int n, int numrows){
    double* yVec = (double*) calloc(numrows, sizeof (double));
    for (int y = 0; y < numrows; ++y) {
        for (int x = 0; x < n; ++x) {
            yVec[y] += A[y*numrows + x] * vec[x];
        }
    }
    return yVec;
}

void powermethod(int my_rank, int n, int nrows, double* x, double* A){
    for (int i = 0; i < 1000; ++i) {
        if (my_rank == 0) normalize(x, n);
        MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double* y = matVec(A, x, n, nrows);

        MPI_Gather(y, nrows, MPI_DOUBLE, x, nrows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        free(y);
    }
}

int main (int argc, char *argv[])
{
    int my_rank, size;
    int snd_buf, rcv_buf;
    int right, left;
    int sum, i;

    int n = 200;
    int nrows;
    double start;

    MPI_Status  status;
    MPI_Request request;


    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* This subproject is about to write a parallel program to
       multiply a matrix A by a vector x, and to use this routine in
       an implementation of the power method to find the absolute
       value of the largest eigenvalue of the matrix. Your code will
       call routines that we supply to generate matrices, record
       timings, and validate the answer.
    */
    double* x = (double*) calloc(n, sizeof(double));
    double* A = hpc_generateMatrix(n, my_rank*nrows, nrows);
    nrows = n/size;

    if (my_rank == 0){
        for (int j = 0; j < n; ++j) {
            x[j] += rand();
        }
        start = hpc_timer();
    }

    powermethod(my_rank, n, nrows, x, A);
    double lambda = norm(x, n);
    if (my_rank == 0) {
        double end = hpc_timer()-start;
        hpc_verify(x, n, end);
        printf("%d,%f,", n, end);
    }
    free(x);
    free(A);
    MPI_Finalize();
    return 0;
}
