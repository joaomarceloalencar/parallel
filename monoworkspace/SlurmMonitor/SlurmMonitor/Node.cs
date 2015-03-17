using System;

namespace SlurmMonitor
{
	public class Node
	{
		private string hostname;
		private double load;
		private DateTime lastupdate;
		private int cores;
		private int memory;

		public string Hostname 
		{
			get
			{
				return hostname;
			}
		}

		public double Load 
		{
			get
			{ 
				return load;
			}
		}

		public Node (string hostname, int cores, int memory)
		{
			this.hostname = hostname;
			this.cores = cores;
			this.memory = memory;
			updateLoad ();
		}

		public void updateLoad ()
		{
			// Atualizar a carga do nó
			lastupdate = DateTime.Now;

		}

		public bool isOverloaded() 
		{
			if (load > cores + 2) 
				return true;
			else
				return false;
		}
	}
}

