#!/bin/bash -l

#SBATCH --job-name=strongScaling
#SBATCH --time=4:00:00
#SBATCH --nodes=1

echo "Grid Size,Threads,Time,Rate"
for i in {1..24} ; do
  export OMP_NUM_THREADS=$i

  for s in 64 128 256 512 1024 ; do
        ./main $s 100 0.005
  done

done