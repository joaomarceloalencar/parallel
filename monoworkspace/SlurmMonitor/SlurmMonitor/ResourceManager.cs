using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace SlurmMonitor
{
	public class ResourceManager
	{
		private Node[] nodeList;
		private DateTime lastupdate;

		public DateTime LastUpdate
		{
			get 
			{
				return lastupdate;
			}
		}

		public ResourceManager ()
		{
			nodeList = getNodes ();
			lastupdate = DateTime.Now;
			// updateNodesLoad ();
		}

		Node[] getNodes ()
		{
			// Recuperar o nome dos nós que estão ativos e adicionar numa nova lista e depois retorná-la.

			// Retira do arquivo do SLURM os nós configurados
			var proc = new Process {
				StartInfo = new ProcessStartInfo {
					FileName = "/bin/bash",
					Arguments = "-c \"cat /etc/slurm/slurm.conf | grep NodeName | cut -f2 -d= | cut -f1 -d' ' | sort -n -t[ -k 2,2\"",
					UseShellExecute = false,
					RedirectStandardOutput = true,
					CreateNoWindow = true
				}
			};

			// Para cada linha retornada, expandir a expressão dos nós. É importante aqui contar a quantidade de nós. 
			proc.Start();
			while (!proc.StandardOutput.EndOfStream) {
				string line = proc.StandardOutput.ReadLine();
				Console.WriteLine (line);
			}

			// Verifica se o nó está ativo.

			return new Node[5];
		}

		void updateNodesLoad ()
		{
			// Para cada nó disponível, atualizar sua carga.
			nodeList = getNodes ();

			for (int i = 0; i < nodeList.Length; i++) 
			{
				nodeList [i].updateLoad ();
			}

			lastupdate = DateTime.Now;
		}

		void printNodesLoad()
		{
			// Imprimir relatório de cargas.
			Console.WriteLine ("### LOAD REPORT ###");
			updateNodesLoad ();
			for (int i = 0; i < nodeList.Length; i++) {
				if (nodeList [i].isOverloaded())
					Console.WriteLine ("{0}:{1} >>> OVERLOAD", nodeList[i].Hostname, nodeList[i].Load);
				else 
					Console.WriteLine ("{0}:{1}", nodeList[i].Hostname, nodeList[i].Load);
			}
		}
	}
}

