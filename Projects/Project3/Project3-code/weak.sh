#!/bin/bash -l

#SBATCH --job-name=weakScaling
#SBATCH --time=0:10:00
#SBATCH --nodes=1

echo "Grid Size,Threads,Time,Rate"
for i in {1..24} ; do
  export OMP_NUM_THREADS=$i
  size=`echo "scale=0; sqrt(1024*1024*$i/24)" | bc`
  ./main $size 100 0.005
done