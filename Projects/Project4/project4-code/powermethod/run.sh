#!/bin/bash -l

#SBATCH --job-name=power
#SBATCH --time=0:20:00
#SBATCH --nodes=32

for i in 1 4 8 12 16 32; do
  mpirun -np $i --oversubscribe ./powermethod
  echo $i
done