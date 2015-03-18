using System;
using System.Diagnostics;

namespace SlurmMonitor
{
	public class Node : IComparable <Node>
	{
		private string hostname;
		private double load;
		private string state;
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

		public string State
		{
			get 
			{
				return state;
			}
		}

		public Node (string hostname, int cores)
		{
			this.hostname = hostname;
			this.cores = cores;
			// updateLoad ();
		}

		// Função para comparar um nó a outro pelo nome. Por exemplo, padufc10 está a frente de padufc2.
		public int CompareTo(Node rhs)
		{ 

			if (hostname.Equals (rhs.Hostname))
				return 0;

			int i;
			for (i = 0; i < hostname.Length && i < rhs.Hostname.Length && hostname.ToCharArray()[i] == rhs.Hostname.ToCharArray()[i]; i++) {}

			if (i == hostname.Length)
				return -1;
			else if (i == rhs.Hostname.Length)
				return 1;
			else if (hostname.ToCharArray () [i] < rhs.Hostname.ToCharArray () [i]) {
				if (hostname.Length > rhs.Hostname.Length)
					return 1;
				else if (hostname.Length <= rhs.Hostname.Length)
					return -1;
			} else if (hostname.ToCharArray () [i] > rhs.Hostname.ToCharArray () [i]) {
				if (hostname.Length >= rhs.Hostname.Length)
					return 1;
				else if (hostname.Length < rhs.Hostname.Length)
					return -1;
			}

			return 0;
		}

		// Atualiza a carga do nó.
		public void updateLoad ()
		{
			// Atualizar a carga do nó
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

		// Verifica se o nó está sobrecarregado.
		public bool isOverloaded() 
		{
			if (load > cores + 2) 
				return true;
			else
				return false;
		}

		// Atualiza o estado do nó.
		public void updateState() {
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
					state = line.Split(separators, 10)[1];
				}
			}
		}
	}
}

