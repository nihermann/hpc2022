from mpi4py import MPI

comm = MPI.COMM_WORLD

size = comm.Get_size()
rank = comm.Get_rank()

# compute a distribution of processes per coordinate direction
dims = [0, 0]
dims = MPI.Compute_dims(size, dims)

# create two-dimensional non-periodic Cartesian topology
periods = [False, False]
comm_cart = comm.Create_cart(dims, periods=periods)

# get rank's coordinates in the topology
coords = comm_cart.Get_coords(rank)

# get rank's south/north/west/east neighbors
neigh_south, neigh_north = comm_cart.Shift(1, 1)
neigh_west, neigh_east = comm_cart.Shift(0, 1)
print(f"Rank {rank}: {neigh_north}/{neigh_south} N/S; {neigh_west}/{neigh_east} W/E")
