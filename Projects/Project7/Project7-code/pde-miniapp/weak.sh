#!/bin/bash -l

#SBATCH --job-name=weakScaling
#SBATCH --time=1:00:00
#SBATCH -n 16
#SBATCH -N 1
#SBATCH --exclusive

module load openmpi

echo "Grid Size,Processes,Time,Rate"
for i in {1..16} ; do
  size=`echo "scale=0; sqrt(1024*1024*$i/24)" | bc`
  mpirun -np $i ./main $size 100 0.005
done