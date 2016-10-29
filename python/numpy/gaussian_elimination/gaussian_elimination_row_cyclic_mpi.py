#!/usr/bin/python
import numpy as np
from mpi4py import MPI
from utils import *

def gaussian_elimination(comm, a, b):
   # MPI rank and comm size
   me = comm.Get_rank()
   p = comm.Get_size()

   # Solutions array, elimination factors and buffer for communication
   x = np.empty(b.size)
   l = np.empty(b.size)
   buf = np.empty(b.size + 1)

   # Create each matrix A[k]
   for k in range(b.size - 1):
      r = max_col_loc(p, me, a, k)
      # print "Rank", rank, "k", k, "r", r
      
      # Find out who has the largest value
      senddata = np.zeros(1, dtype='d')
      if r != -1 :
         senddata[0] = abs(a[r,k])
      else :
         senddata[0] = 0 
      recvdata = np.zeros(p, dtype='d')
      comm.Allgather([senddata,  MPI.DOUBLE], [recvdata, MPI.DOUBLE])
      node = np.argmax(recvdata)
      print "Rank", rank, "k", k, "r", r, "node", node

   return x

if __name__ == "__main__" :

   # Start MPI Environment
   comm = MPI.COMM_WORLD
   rank = comm.Get_rank()
   size = comm.Get_size()
   print "Starting process " + str(rank) + " of " + str(size)

   # Load the Matrices
   matrix_file = "data/dezpordez.mtx"
   matrix_file_b = "data/dezpordez_b.mtx"
   matrix = load_mtx(matrix_file)
   matrix_b = load_mtx(matrix_file_b)

   if rank == 0:
      print matrix.toarray()

   # Perform the elimination
   x = gaussian_elimination(comm, matrix, matrix_b)
