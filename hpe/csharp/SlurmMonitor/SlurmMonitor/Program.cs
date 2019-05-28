using System;
using System.Collections.Generic;

namespace SlurmMonitor
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			ResourceManager rs = new ResourceManager ();
			rs.printNodesLoad ();
		}
	}
}
