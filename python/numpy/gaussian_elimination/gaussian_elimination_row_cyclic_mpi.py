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
      val = recvdata[0]
      # print "Rank", rank, "k", k, "r", r, "node", node
      
      if k % p == node :
      # The pivot row and row k are on the same processor
         if k % p == me :
            if a[k,k] != val :
               exchange_row(a, b, r, k)
            buf = copy_row(a, b, k)
      else :
      # The pivot row and row k are owned by diffent processors
         if k % p == me :
            # row k belongs to this process
            buf = copy_row(a, b, k)
            comm.Send([buf, MPI.DOUBLE], dest = node, tag = 77)
         elif node == me :
            # the pivot row belongs to this process
            comm.Recv([buf, MPI.DOUBLE], source = MPI.ANY_SOURCE, tag = 77)
	    copy_exchange_row(a, b, r, buf, k)
      # Broadcast the pivot row to everyone	    
      comm.Bcast(buf, root=node)
      # If I had row k, let me replace that for the pivot row
      if ((k % p != node) and (k % p == me)):
         copy_back_row(a, b, buf, k) 
      # Let's jump to the current row this process should take care	 
      row = k + 1
      while (row % p != me):
         row++
      # Update my rows accordingly the pivot row	 
      for i in range(row, b.size, p):
         l[i] = a[i,k] / buf[k]
	 for j in range(k+1, b.size):
	    a[i,j] = a[i,j] - l[i] * buf[j]
	 b[i] = b[i] - l[i] * buf[n]
    # Not that we reached A[k], let's do the backward substitution
    for k in reversed(range(n, 0)):
       if (k % p == me):
          _sum = 0.0
	  for j in range(k+1, n):
	     _sum = _sum + a[k,j] * x[j]
	  x[k] = 1 / a[k,k] * (b[k] - _sum)
       comm.Bcast(x[k], root=k % p)
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
