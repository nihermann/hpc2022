#!/bin/bash -l

#SBATCH --job-name=strongScaling
#SBATCH --time=1:00:00
#SBATCH -n 16
#SBATCH --exclusive

module load openmpi

echo "Grid Size,Processes,Time,Rate"
for i in {1..16} ; do
  for s in 64 128 256 512 1024 ; do
    mpirun -np $i ./main $s 100 0.005
  done
done