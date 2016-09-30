#!/usr/bin/python
from mpi4py import MPI
import numpy 

def matvec(comm, A, X):
   m = A.shape[0]
   p = comm.Get_size()
   xg = numpy.zeros(m * p, dtype = 'd')
   comm.Allgather([x, MPI.DOUBLE],
                  [xg, MPI.DOUBLE])
   y = numpy.dot(A, xg)
   return y
