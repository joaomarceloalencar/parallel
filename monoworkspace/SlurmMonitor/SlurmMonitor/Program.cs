using System;
using System.Collections.Generic;

namespace SlurmMonitor
{
	class MainClass
	{
		public static void Main (string[] args)
		{

			ResourceManager rs = new ResourceManager ();
			rs.updateNodesLoad ();	
			rs.printNodesLoad ();

			/*
			List<Node> nodelist = new List<Node> ();

			for (int i = 48; i > 0; i--)
				nodelist.Add(new Node("padufc" + i, 12));

			nodelist.Add (new Node ("padufc101", 16));		
			nodelist.Add (new Node ("padufc102", 16));		
			nodelist.Add (new Node ("padufc103", 16));		

			nodelist.Sort ();

			foreach (Node n in nodelist) 
				Console.WriteLine (n.Hostname);
			*/
		}
	}
}
