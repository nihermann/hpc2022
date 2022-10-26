#!/bin/bash -l

#SBATCH --job-name=hist
#SBATCH --time=01:00:00
#SBATCH --nodes=1

./hist_seq

for i in {1..24} ; do
  export OMP_NUM_THREADS=$i
  ./hist_omp
done
