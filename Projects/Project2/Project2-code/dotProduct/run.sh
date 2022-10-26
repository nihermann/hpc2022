#!/bin/bash -l

#SBATCH --job-name=dotProduct
#SBATCH --time=13:00:00
#SBATCH --nodes=1

for i in {1..24} ; do
  export OMP_NUM_THREADS=$i
  ./dotProduct
done
