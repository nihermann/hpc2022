#!/bin/bash -l

#SBATCH --job-name=weakScaling
#SBATCH --time=1:00:00
#SBATCH -N 16
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive

conda activate mpi

echo "Grid Size,Processes,Time,Rate"
for i in {1..16} ; do
  size=`echo "scale=0; sqrt(1024*1024*$i/24)" | bc`
  mpiexec -n $i python main.py $size 100 0.005
done