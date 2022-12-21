#!/bin/bash -l

#SBATCH --job-name=strongScaling
#SBATCH --time=1:00:00
#SBATCH -N 16
#SBATCH --ntasks-per-node=1

conda activate mpi

echo "Grid Size,Processes,Time,Rate"
for i in {1..16} ; do
  for s in 64 128 256 512 1024 ; do
    mpiexec -n $i python main.py $s 100 0.005
  done
done