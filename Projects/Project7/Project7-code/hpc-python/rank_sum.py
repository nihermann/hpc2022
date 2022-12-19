import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

# slow version with native python
# result = comm.allreduce(rank, MPI.SUM)

# fast version with numpy
result = np.array(0)
comm.Allreduce(np.array(rank), result, MPI.SUM)

print(f"Rank {rank} got rank sum {result}")
