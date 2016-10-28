#!/usr/bin/python
from mpi4py import MPI
from utils import *

def gaussian_elimination(p, rank, a, b):

if __name__ == "__main__" :
   comm = MPI.COMM_WORLD
   rank = comm.Get_rank()
   size = comm.Get_size()
   print "Hello world from " + str(rank) + " " + str(size)
