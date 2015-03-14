using System;
using System.Collections.Generic;
using MPI;
 
class Hello{
    static void Main(string[] args){
        IDictionary<int, MPI.Intracommunicator> colors = new Dictionary<int, MPI.Intracommunicator>();
 
        MPI.Environment mpi = new MPI.Environment(ref args);
        MPI.Intracommunicator worldComm = Communicator.world;
        int min = 8; int max=min; int rank = min;
 
        rank = worldComm.Rank;
        min = worldComm.Allreduce<int>(rank+1, MPI.Operation<int>.Min);
        max = worldComm.Reduce<int>(rank+1, MPI.Operation<int>.Max, 1);
 
        colors[rank] = (MPI.Intracommunicator)worldComm.Split(rank+100, rank);
 
        int rec = 8;
        if (rank == 0) {
            MPI.Request req = worldComm.ImmediateSend<int> (50, rank, 0);
            rec = worldComm.Receive<int> (rank, 0);
            req.Wait();
        }
        Console.WriteLine("Node {0} of {1} - min={2} - max={3} - rec={4}\n", rank, worldComm.Size, min,  max, rec);
        mpi.Dispose();
   }
}
