using System;
using System.Collections.Generic;
using MPI;

class Hello{
    static void Main(string[] args){
        MPI.Environment mpi = new MPI.Environment(ref args);
        MPI.Intracommunicator worldComm = Communicator.world;

        Console.WriteLine("OK\n");
        mpi.Dispose();
     }
}

