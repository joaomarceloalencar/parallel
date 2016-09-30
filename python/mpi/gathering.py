#!/usr/bin/python
from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

data = (rank + 1) ** 2
data = comm.gather(data, root = 0)
if rank == 0:
   for i in range(size):
      print data[i]
else:
   data = None



