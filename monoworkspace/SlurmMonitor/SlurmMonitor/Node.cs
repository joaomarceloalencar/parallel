using System;
using System.Diagnostics;

namespace SlurmMonitor
{
	public class Node
	{
		private string hostname;
		private double load;
		private DateTime lastupdate;
		private int cores;

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

		public DateTime LastUpdate
		{
			get 
			{
				return lastupdate;
			}
		}

		public Node (string hostname, int cores)
		{
			this.hostname = hostname;
			this.cores = cores;
			// updateLoad ();
		}

		public void updateLoad ()
		{
			// Atualizar a carga do nó
			lastupdate = DateTime.Now;

			// Recupera informação do SLURM sobre a carga do nó
			var proc = new Process {
				StartInfo = new ProcessStartInfo {
					FileName = "pdsh",
					Arguments = " -w " + hostname + " cat /proc/loadavg ",
					UseShellExecute = false,
					RedirectStandardOutput = true,
					CreateNoWindow = true
				}
			};
			// Retorna apenas uma linha.
			proc.Start();
			while (!proc.StandardOutput.EndOfStream) {
				string line = proc.StandardOutput.ReadLine();
				char[] separators = {' '};
				load = double.Parse(line.Split(separators,10)[1]);
			}
		}

		public bool isOverloaded() 
		{
			if (load > cores + 2) 
				return true;
			else
				return false;
		}

		public bool isActive() {
			// Recupera informação do SLURM sobre o estado do nó
			var proc = new Process {
				StartInfo = new ProcessStartInfo {
					FileName = "scontrol",
					Arguments = "show node=" + hostname,
					UseShellExecute = false,
					RedirectStandardOutput = true,
					CreateNoWindow = true
				}
			};
			// Para cada linha retornada, procura a que tem o estado do nó.
			proc.Start();
			while (!proc.StandardOutput.EndOfStream) {
				string line = proc.StandardOutput.ReadLine();
				line = line.Trim ();

				if (line.StartsWith ("State")) {
					char[] separators = {' ', '='};
					string status = line.Split(separators, 10)[1];
					if (status.Equals("DOWN*"))
					    return false;
				}
			}
			return true;
		}
	}
}

