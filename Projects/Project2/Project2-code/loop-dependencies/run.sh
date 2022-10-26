#!/bin/bash -l

#SBATCH --job-name=loops
#SBATCH --time=00:15:00
#SBATCH --nodes=1

./recur_seq
export OMP_NUM_THREADS=16
./recur_omp
