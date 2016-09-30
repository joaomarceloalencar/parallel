#!/usr/bin/python
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0 :
   data = np.arange(10, dtype='i')
else :
   data = np.empty(10, dtype='i')

comm.Bcast(data, root = 0)

for i in range(10):
   print "Rank: " + str(rank) + " data[b]:" + str(data[i])

