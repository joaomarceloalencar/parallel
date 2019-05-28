using System;
using MPI;

class Ring
{
	static void Main(string [] args)
	{
		using (new MPI.Environment(ref args))
		{
			Intracommunicator comm = Communicator.world;
			if (comm.Rank == 0)
			{
				// program for rank 0
				comm.Send("JM", 1, 0);
				string msg = comm.Receive<string>(Communicator.anySource, 0);
				Console.WriteLine("Rank {0} received message {1}.", comm.Rank, msg);
			}
			else 
			{
				// program for all other ranks
				string msg = comm.Receive<string>(comm.Rank - 1, 0);
				Console.WriteLine("Rank	{0} received message {1}.", comm.Rank, msg);
				comm.Send(msg + ", " + comm.Rank, (comm.Rank + 1) % comm.Size, 0);
			}
		}
	}
}
