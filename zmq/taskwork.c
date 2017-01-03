//  Task worker
//  Connects PULL socket to tcp://localhost:5557
//  Collects workloads from ventilator via that socket
//  Connects PUSH socket to tcp://localhost:5558
//  Sends results to sink via that socket

#include "zhelpers.h"
#include <mpi.h>

int main (int argc, char *argv[])
{
    //  MPI
    int rank, size;

    MPI_Init (&argc, &argv);	/* starts MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
   
    //  Socket to receive messages on
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_PULL);
    zmq_connect (receiver, "tcp://localhost:5557");

    //  Socket to send messages to
    void *sender = zmq_socket (context, ZMQ_PUSH);
    zmq_connect (sender, "tcp://localhost:5558");

    while (1) {
       char *string = s_recv (receiver);
       char buffer[100];
       printf ("Rank %d Receive %s.\n", rank, string); 
       fflush (stdout);
       sprintf(buffer, "From Rank %d Receive %s\n", rank, string);
       s_send(sender, buffer);
       free (string);
    }
    
    zmq_close (receiver);
    zmq_close (sender);
    zmq_ctx_destroy (context);

    MPI_Finalize();

    return 0;
}

