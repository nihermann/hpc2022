#include <omp.h>
#include "walltime.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.008s ~0.8MB
//#define N 100000
// 0.1s ~8MB
// #define N 1000000
// 1.1s ~80MB
// #define N 10000000
// 13s ~800MB
// #define N 100000000
// 127s 16GB
 #define N 1000000000
#define EPSILON 0.1

using namespace std;

int main() {
    double time_serial, time_start = 0.0;
    double *a, *b;
    int num_threads = omp_get_max_threads();

    // Allocate memory for the vectors as 1-D arrays
    a = new double[N];
    b = new double[N];

    // Initialize the vectors with some values
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i / 10.0;
    }

    long double alpha = 0;
    // serial execution
    // Note that we do extra iterations to reduce relative timing overhead
    time_start = wall_time();
    for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
        alpha = 0.0;
        for (int i = 0; i < N; i++) {
            alpha += a[i] * b[i];
        }
    }
    time_serial = wall_time() - time_start;
//    cout << "Serial execution time = " << time_serial << " sec" << endl;
//    cout << "Serial dot product = " << alpha << endl;
    cout << N << "," << num_threads << ",serial," << time_serial << endl;

    long double alpha_parallel = 0;

    //   TODO: Write parallel version (2 ways!)
    //   i.  Using reduction pragma
    time_start = wall_time();
        for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
            alpha_parallel = 0.0;
#pragma omp reduction(+: alpha_parallel)
    {
            for (int i = 0; i < N; i++) {
                alpha_parallel += a[i] * b[i];
            }
        }
    }
    double time_red = wall_time() - time_start;

    if ((fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
        cout << "!!! parallel reduction: " << alpha_parallel << ", serial: " << alpha
             << "\n";
        cerr << "Alpha not yet implemented correctly!\n";
        exit(1);
    }

    //   ii. Using  critical pragma
    time_start = wall_time();
    for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
        alpha_parallel = 0.0;
#pragma omp parallel for
        for (int i = 0; i < N; i++) {
#pragma omp critical
            alpha_parallel += a[i] * b[i];
        }
    }
    double time_critical = wall_time() - time_start;

    if ((fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
        cout << "!!! parallel critical: " << alpha_parallel << ", serial: " << alpha
             << "\n";
        cerr << "Alpha not yet implemented correctly!\n";
        exit(1);
    }
//    cout << "Parallel dot product = " << alpha_parallel << endl
//         << "time using reduction method = " << time_red << " sec" << endl
//         << "time using critical method " << time_critical << " sec"
//         << endl;
    cout << N << "," << num_threads << ",reduction," << time_red << endl;
    cout << N << "," << num_threads << ",critical," << time_critical << endl;

    // De-allocate memory
    delete[] a;
    delete[] b;

    return 0;
}
