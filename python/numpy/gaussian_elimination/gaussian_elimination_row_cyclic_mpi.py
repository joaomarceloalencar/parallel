#!/usr/bin/python
import numpy as np
from mpi4py import MPI
from utils import *

def gaussian_elimination(comm, a, b):
   # MPI rank and comm size
   me = comm.Get_rank()
   p = comm.Get_size()

   # Solutions array, elimination factors and buffer for communication
   x = np.zeros(b.size)
   l = np.zeros(b.size)
   buf = np.zeros(b.size + 1)

   # Create each matrix A[k]
   for k in range(b.size - 1):
      r = max_col_loc(p, me, a, k)
      #print "Rodada: ", k, "Processo: ", me, "Maior coluna:", r

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
      
      #print "Rodada: ", k, "Processo: ", me, "Processo com maior valor", node, "Valor: ", val
      
      if k % p == node :
      # The pivot row and row k are on the same processor
         if k % p == me :
            if a[k,k] != val :
               #print "Rodada: ", k, "Processo: ", me, "a[k][k] ", a[k,k], "Valor: ", val
               #print "Rodada: ", k, "A: ", a.toarray()
               exchange_row(a, b, r, k)
               #print "Rodada: ", k, "A: ", a.toarray()
            buf = copy_row(a, b, k)
            #print "Rodada: ", k, "buf: ", buf 
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
      comm.Bcast(buf, root = node)
      #print "Rodada: ", k, "Processo: ", me, "buf apos broadcast: ", buf 
      # If I had row k, let me replace that for the pivot row
      if ((k % p != node) and (k % p == me)):
         copy_back_row(a, b, buf, k) 

      #print "Rodada: ", k, "Processo: ", me, "A apos broadcast:", a.toarray() 
      # Let's jump to the current row this process should take care	 
      row = k + 1 
      while (row % p != me):
         row = row + 1
      #print "Rodada: ", k, "Processo: ", me, "Vou comecar na linha: ", row
      # Update my rows accordingly the pivot row	 
      for i in range(row, b.size, p):
         l[i] = a[i,k] / buf[k]
	 for j in range(k, b.size):
	    a[i,j] = a[i,j] - l[i] * buf[j]
	 b[i] = b[i] - l[i] * buf[b.size]
      print "Rodada: ", k, "Processo: ", me, "A apos multiplicacao:", np.around(a.toarray(), decimals=2)
      print "Rodada: ", k, "Processo: ", me, "B apos multiplicacao:", np.around(b, decimals=2)
   
   # Not that we reached A[k], let's do the backward substitution
   for k in reversed(range(0, b.size)):
      if (k % p == me):
         _sum = 0.0
         for j in range(k+1, b.size):
            _sum = _sum + a[k,j] * x[j]
            x[k] = 1 / a[k,k] * (b[k] - _sum)
      # Let's share the new value of x[k] to everyone
      value = np.arange(1)
      value[0] = x[k]
      comm.Bcast(value, root = k % p)
      x[k] = value[0]
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
      print matrix_b

   # Perform the elimination
   x = gaussian_elimination(comm, matrix, matrix_b)

   if rank == 0:
      print x
