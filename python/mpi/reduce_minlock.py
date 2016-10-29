# reduce_array
from mpi4py import MPI
import numpy

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

inp = numpy.random.rand(size)
senddata = inp[rank]
recvdata=comm.reduce(senddata,None,root=0,op=MPI.MINLOC)
print 'on task',rank,'reduce:    ',senddata,recvdata
 
recvdata=comm.allreduce(senddata,None,op=MPI.MINLOC)
print 'on task',rank,'allreduce: ',senddata,recvdata


