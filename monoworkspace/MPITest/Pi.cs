using System;
using MPI;

class Pi
{
	static void Main(string[] args)
	{
		using (new MPI.Environment(ref args))
		{
			Intracommunicator comm = Communicator.world;
			int dartsPerProcessor = 100000000;
			Random random = new Random(5 * comm.Rank);
			int dartsInCircle = 0;
			for (int i = 0; i < dartsPerProcessor; ++i)
			{
				double x = (random.NextDouble() - 0.5) * 2;
				double y = (random.NextDouble() - 0.5) * 2;
				if (x * x + y * y <= 1.0)
					++dartsInCircle;
			}

			int totalDartsInCircle = comm.Reduce(dartsInCircle, Operation<int>.Add, 0);
			if (comm.Rank == 0)
				Console.WriteLine("Pi is approximately {0:F15}", 4 * (double) totalDartsInCircle / (comm.Size * (double) dartsPerProcessor));
		}
	}
}
