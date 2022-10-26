#!/bin/bash -l

#SBATCH --job-name=mandel
#SBATCH --time=00:30:00
#SBATCH --nodes=1

export OMP_NUM_THREADS=24
./mandel_omp