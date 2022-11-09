//******************************************
// operators.f90
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 3d-meshes

#include <iostream>
#include "data.h"
#include "operators.h"
#include "stats.h"
#include "omp.h"
#include "immintrin.h"

namespace operators {

// input: s, gives updated solution for f
// only handles interior grid points, as boundary points are fixed
// those inner grid points neighbouring a boundary point, will in the following
// be referred to as boundary points and only those grid points
// only neighbouring non-boundary points are called inner grid points
void diffusion(const data::Field &s, data::Field &f)
{
    using data::options;

    using data::bndE;
    using data::bndW;
    using data::bndN;
    using data::bndS;

    using data::y_old;

    double alpha = options.alpha;
    double beta = options.beta;

    int nx = options.nx;
    int iend  = nx - 1;
    int jend  = nx - 1;

    // the interior grid points
    // make all constants
    __m256d _four, _one, _mFourPlusAlpha, _alpha, _beta;
    _four = _mm256_set1_pd(4);
    _one = _mm256_set1_pd(1);
    _mFourPlusAlpha = _mm256_set1_pd(-4+alpha);
    _alpha = _mm256_set1_pd(alpha);
    _beta = _mm256_set1_pd(beta);

#pragma omp parallel for collapse(2)
    for (int j=1; j < jend; j++) {
        for (int i=1; i < iend; ++i) {//i+=4) {
//            if (i + 4 >= iend){
//                for (; i < iend; ++i) {
//                    f(i,j) = -(4. + alpha) * s(i,j) + s(i-1,j) + s(i+1,j)  // (1)
//                    + s(i,j-1) + s(i,j+1) + beta*s(i,j) *  (1 - s(i,j)) // (2)
//                    + alpha*y_old(i,j); // (3)
//                }
//            }else{
//
//                auto _sij = _mm256_loadu_pd(&s(i,j));
//                auto _yold = _mm256_loadu_pd(&y_old(i,j));
//                auto _smij = _mm256_loadu_pd(&s(i+1,j));
//                auto _spij = _mm256_loadu_pd(&s(i+1,j));
//                auto _simj = _mm256_loadu_pd(&s(i,j-1));
//                auto _sipj = _mm256_loadu_pd(&s(i,j+1));
//
//                auto _eq1 = _mm256_fmadd_pd(
//                        _mFourPlusAlpha, _sij,
//                        _mm256_add_pd(_smij, _spij)
//                        ); // -(4. + alpha) * s(i,j) + s(i-1,j) + s(i+1,j)
//
//                auto _betaTsij = _mm256_mul_pd(_beta, _sij);
//                auto _oneTsij = _mm256_sub_pd(_one, _sij);
//                auto _eq2 = _mm256_fmadd_pd(
//                        _betaTsij, _oneTsij,
//                        _mm256_add_pd(_simj, _sipj)
//                        ); // s(i,j-1) + s(i,j+1) + beta*s(i,j) *  (1 - s(i,j))
//                auto _eq3Peq2 = _mm256_fmadd_pd(_alpha, _yold, _eq2);
//                auto _res = _mm256_add_pd(_eq1, _eq3Peq2);
//
//                _mm256_storeu_pd(&f(i,j), _res);
//            }

            //TODO
            f(i,j) = -(4. + alpha) * s(i,j) + s(i-1,j) + s(i+1,j)  // eq (1)
                    + s(i,j-1) + s(i,j+1) + beta*s(i,j) *  (1 - s(i,j)) // eq (2)
                    + alpha*y_old(i,j); // eq (3)
        }
    }

    // the east boundary
    {
        int i = nx - 1;
#pragma omp parallel for
        for (int j = 1; j < jend; j++)
        {
            f(i,j) = -(4. + alpha) * s(i,j)
                        + s(i-1,j) + s(i,j-1) + s(i,j+1)
                        + alpha*y_old(i,j) + bndE[j]
                        + beta * s(i,j) * (1.0 - s(i,j));
        }
    }

    // the west boundary
    {
        int i = 0;
        //TODO
#pragma omp parallel for
        for (int j = 1; j < jend; ++j) {
            f(i,j) = -(4. + alpha) * s(i,j)
                     + s(i+1,j) + s(i,j-1) + s(i,j+1)
                     + alpha*y_old(i,j) + bndW[j]
                     + beta * s(i,j) * (1.0 - s(i,j));
        }

    }

    // the north boundary (plus NE and NW corners)
    {
        int j = nx - 1;

        {
            int i = 0; // NW corner
            f(i,j) = -(4. + alpha) * s(i,j)
                        + s(i+1,j) + s(i,j-1)
                        + alpha * y_old(i,j) + bndW[j] + bndN[i]
                        + beta * s(i,j) * (1.0 - s(i,j));
        }

        // inner north boundary
        //TODO
        {
#pragma omp parallel for
            for (int i = 1; i < iend; ++i) {
                f(i,j) = -(4. + alpha) * s(i,j)
                         + s(i+1,j) + s(i,j-1) + s(i-1,j)
                         + alpha*y_old(i,j) + bndN[i]
                         + beta * s(i,j) * (1.0 - s(i,j));
            }
        }

        {
            int i = nx-1; // NE corner
            f(i,j) = -(4. + alpha) * s(i,j)
                        + s(i-1,j) + s(i,j-1)
                        + alpha * y_old(i,j) + bndE[j] + bndN[i]
                        + beta * s(i,j) * (1.0 - s(i,j));
        }
    }

    // the south boundary
    {
        int j = 0;

        {
            int i = 0; // SW corner
            f(i, j) = -(4. + alpha) * s(i, j)
                      + s(i + 1, j) + s(i, j + 1)
                      + alpha * y_old(i, j) + bndW[j] + bndS[i]
                      + beta * s(i, j) * (1.0 - s(i, j));
        }

        // inner south boundary
        //TODO
        {
#pragma omp parallel for
            for (int i = 1; i < iend; ++i) {
                f(i,j) = -(4. + alpha) * s(i,j)
                         + s(i+1,j) + s(i-1,j) + s(i,j+1)
                         + alpha*y_old(i,j) + bndS[i]
                         + beta * s(i,j) * (1.0 - s(i,j));
            }
        }

        {
            int i = nx - 1; // SE corner
            f(i,j) = -(4. + alpha) * s(i,j)
                        + s(i-1,j) + s(i,j+1)
                        + alpha * y_old(i,j) + bndE[j] + bndS[i]
                        + beta * s(i,j) * (1.0 - s(i,j));
        }
    }

    // Accumulate the flop counts
    // 8 ops total per point
    stats::flops_diff +=
        + 12 * (nx - 2) * (nx - 2) // interior points
        + 11 * (nx - 2  +  nx - 2) // NESW boundary points
        + 11 * 4;                                  // corner points
}

} // namespace operators
