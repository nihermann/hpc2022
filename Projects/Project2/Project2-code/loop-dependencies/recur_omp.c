#include "walltime.h"
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  int N = 2000000000;
  double up = 1.00000001;
  double Sn = 1.00000001;
  int n;
  /* allocate memory for the recursion */
  double *opt = (double *)malloc((N + 1) * sizeof(double));

  if (opt == NULL)
    die("failed to allocate problem size");

  double time_start = wall_time();
  // TODO: YOU NEED TO PARALLELIZE THIS LOOP
  int block = N / (omp_get_max_threads()*2);
  int bound = N + block - 1;
#pragma omp parallel for schedule(dynamic) firstprivate(Sn) lastprivate(Sn)
    for (int n = 0; n < bound; n += block) {
        int end = n + block > N? N : n + block;
        Sn = pow(up, n+1);
        opt[n] = Sn;
        for (int i = n + 1; i < end; ++i) {
            Sn *= up;
            opt[i] = Sn;
        }
  }

  printf("Parallel RunTime   :  %f seconds\n", wall_time() - time_start);
  printf("Final Result Sn    :  %.17g \n", Sn);

  double temp = 0.0;
  for (n = 0; n <= N; ++n) {
    temp += opt[n] * opt[n];
  }
  printf("Result ||opt||^2_2 :  %f\n", temp / (double)N);
  printf("\n");

  return 0;
}
