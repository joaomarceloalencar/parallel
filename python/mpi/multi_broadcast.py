#!/usr/bin/python 
from mpi4py import MPI
import random
import numpy

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

recvdata = numpy.zeros(size, dtype='d')
senddata = numpy.zeros(1, dtype='d')
senddata[0] = random.random()

print "Rank", rank, "Sending", senddata[0]

comm.Allgather([senddata,  MPI.DOUBLE], [recvdata, MPI.DOUBLE])

print "Rank", rank, "Received", recvdata, "Max Value", numpy.argmax(recvdata)

