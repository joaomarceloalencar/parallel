using System;

namespace SlurmMonitor
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			ResourceManager rs = new ResourceManager ();
			rs.updateNodesLoad ();	
			rs.printNodesLoad ();
			// Node padufc16 = new Node ("padufc16", 12);
			// Node padufc48 = new Node ("padufc48", 12);

			//if (padufc16.CompareTo(padufc48) == 1)
			//   Console.WriteLine (padufc16.CompareTo(padufc48));
		}
	}
}
