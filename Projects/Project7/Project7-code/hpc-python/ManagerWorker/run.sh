#!/bin/bash -l

#SBATCH --job-name=strongScaling
#SBATCH --time=0:07:00
#SBATCH -n 16
#SBATCH --exclusive

conda activate mpi

echo "Processes,nJobs,Time"
for i in 2 4 8 16 ; do
  for s in 50 100 ; do
    mpiexec -n $i python manager_worker.py 4001 4001 $s
  done
done