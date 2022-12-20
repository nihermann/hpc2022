print("Starting script")
from mandelbrot_task import *
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from mpi4py import MPI # MPI_Init and MPI_Finalize automatically called
import numpy as np
import sys
import time

# some parameters
MANAGER = 0       # rank of manager
TAG_TASK      = 1 # task       message tag
TAG_TASK_DONE = 2 # tasks done message tag
TAG_DONE      = 3 # done       message tag
TasksDoneByWorker = []

def manager(comm, tasks):
    """
    The manager.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    tasks : list of objects with a do_task() method perfroming the task
        List of tasks to accomplish

    Returns
    -------
    ... ToDo ...
    """

    # send initial tasks
    for i in range(1, comm.Get_size()):
        comm.isend(tasks[i-1], dest=i, tag=TAG_TASK)

    buffers = [0] * (comm.Get_size()-1)
    finished_tasks = []
    TasksDoneByWorker = [0] * comm.Get_size()

    recv_reqs = [comm.irecv(buffers[i-1], source=i, tag=TAG_TASK_DONE) for i in range(1, comm.Get_size())]

    # ignore tasks that were already send
    for task in tasks[comm.Get_size()-1:]:
        done_idx, _ = MPI.Request.waitany(recv_reqs)
        print(f"Task finished by worker {done_idx}")
        TasksDoneByWorker[done_idx+1] += 1

        finished_tasks.append(buffers[done_idx])
        comm.isend(task, dest=done_idx, tag=TAG_TASK)
        recv_reqs[done_idx] = comm.irecv(buffers[done_idx], source=done_idx, tag=TAG_TASK_DONE)
    print("All task distributed")
    # wait until the last tasks finished
    finished_tasks += MPI.Request.waitall(recv_reqs)
    # signal workers that there are no more tasks left
    send_done_reqs = [comm.isend(TAG_DONE, dest=i, tag=TAG_TASK) for i in range(1, comm.Get_size())]
    MPI.Request.Waitall(send_done_reqs)

    return finished_tasks


def worker(comm):

    """
    The worker.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    """
    while True:
        task = comm.recv(source=MANAGER, tag=TAG_TASK)
        print(f"Rank {comm.Get_rank()} received task {task}.")

        if task == TAG_DONE:
            break

        task.do_work()

        print(f"Rank {comm.Get_rank()} completed task and is sending results back.")
        comm.send(task, dest=MANAGER, tag=TAG_TASK_DONE)


def readcmdline(rank):
    """
    Read command line arguments

    Parameters
    ----------
    rank : int
        Rank of calling MPI process

    Returns
    -------
    nx : int
        number of gridpoints in x-direction
    ny : int
        number of gridpoints in y-direction
    ntasks : int
        number of tasks
    """
    # report usage
    if len(sys.argv) != 4:
        if rank == MANAGER:
            print("Usage: manager_worker nx ny ntasks")
            print("  nx     number of gridpoints in x-direction")
            print("  ny     number of gridpoints in y-direction")
            print("  ntasks number of tasks")
        sys.exit()

    # read nx, ny, ntasks
    nx = int(sys.argv[1])
    if nx < 1:
        sys.exit("nx must be a positive integer")
    ny = int(sys.argv[2])
    if ny < 1:
        sys.exit("ny must be a positive integer")
    ntasks = int(sys.argv[3])
    if ntasks < 1:
        sys.exit("ntasks must be a positive integer")

    return nx, ny, ntasks


if __name__ == "__main__":
    print("Starting")
    # get COMMON WORLD communicator, size & rank
    comm    = MPI.COMM_WORLD
    size    = comm.Get_size()
    my_rank = comm.Get_rank()

    # report on MPI environment
    if my_rank == MANAGER:
        print(f"MPI initialized with {size:5d} processes")

    # read command line arguments
    nx, ny, ntasks = readcmdline(my_rank)

    # start timer
    timespent = - time.perf_counter()

    # trying out ... YOUR MANAGER-WORKER IMPLEMENTATION HERE ...
    x_min = -2.
    x_max  = +1.
    y_min  = -1.5
    y_max  = +1.5

    if my_rank != MANAGER:
        worker(comm)
    else:
        M = mandelbrot(x_min, x_max, nx, y_min, y_max, ny, ntasks)
        tasks = M.get_tasks()
        tasks = manager(comm, tasks)

        m = M.combine_tasks(tasks)
        plt.imshow(m.T, cmap="gray", extent=[x_min, x_max, y_min, y_max])
        plt.savefig("mandelbrot.png")

    # stop timer
    timespent += time.perf_counter()

    # inform that done
    if my_rank == MANAGER:
        print(f"Run took {timespent:f} seconds")
        for i in range(size):
            if i == MANAGER:
                continue
            print(f"Process {i:5d} has done {TasksDoneByWorker[i]:10d} tasks")
        print("Done.")

