#!/bin/bash -l

#SBATCH --job-name=scaling
#SBATCH --time=2:10:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

module load gcc openmpi

echo "Grid Size,Threads,Time,Ranks"
for i in {1..10} ; do
  export OMP_NUM_THREADS=$i
  for s in 128 256 512 1024 ; do
    mpirun -n 1 --map-by socket:pe=$i --bind-to core ./main $s $s 100 0.01
  done
done