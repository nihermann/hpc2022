#!/bin/bash -l

#SBATCH --job-name=param_search
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --output=param_search-%j.out
#SBATCH --error=param_search-%j.err

# load modules
if command -v module 1>/dev/null 2>&1; then
   module load gcc/10.1.0 intel-mkl/2020.1.217-gcc-10.1.0-qsctnr6 gnuplot likewid
fi

export OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1
likwid-topology
echo "==== benchmark-blocked block size search ===================="
./benchmark-blocked | tee timing_blocked_dgemm.data
