from mpi4py import MPI
import numpy as np

# 2.2
comm = MPI.COMM_WORLD

size = comm.Get_size()
rank = comm.Get_rank()

# compute a distribution of processes per coordinate direction
dims = [0, 0]
dims = MPI.Compute_dims(size, dims)

# create two-dimensional non-periodic Cartesian topology
periods = [True, True]
comm_cart = comm.Create_cart(dims, periods=periods)

# get rank's coordinates in the topology
coords = comm_cart.Get_coords(rank)

# get rank's south/north/west/east neighbors
neigh_north, neigh_south = comm_cart.Shift(1, 1)
neigh_west, neigh_east = comm_cart.Shift(0, 1)
print(f"Rank {rank} at {coords}: {neigh_north}/{neigh_south} N/S; {neigh_west}/{neigh_east} W/E")

# 2.3
up, down, left, right = np.array(0), np.array(0), np.array(0), np.array(0)
rank = np.array(rank)
requests = [
    comm.Irecv(up, neigh_north, neigh_north),
    comm.Irecv(down, neigh_south, neigh_south),
    comm.Irecv(left, neigh_west, neigh_west),
    comm.Irecv(right, neigh_east, neigh_east),

    comm.Isend(rank, neigh_north, rank),
    comm.Isend(rank, neigh_south, rank),
    comm.Isend(rank, neigh_west, rank),
    comm.Isend(rank, neigh_east, rank),
]

MPI.Request.Waitall(requests)

print(f"Rank {rank}: {up}/{down} N/S, {left}/{right} W/E")
