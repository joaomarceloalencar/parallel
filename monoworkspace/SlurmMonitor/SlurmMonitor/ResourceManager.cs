using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace SlurmMonitor
{
	public class ResourceManager
	{
		private List<Node> nodeList;
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
			nodeList = new List<Node> ();
			retrieveNodes ();
			lastupdate = DateTime.Now;
			// updateNodesLoad ();
		}

		public void retrieveNodes ()
		{
			// Recuperar o nome dos nós que estão ativos e adicionar numa nova lista e depois retorná-la.

			// Limpar a lista anterior
			nodeList.Clear ();

			// Abre o arquivo /etc/slurm/slurm.conf para leitura apenas.
			System.IO.StreamReader fileReader = new System.IO.StreamReader ("/etc/slurm/slurm.conf");

			// Percorre linha por linha
			string line;
			while((line = fileReader.ReadLine()) != null) {
				// Verifica se as linhas começam por NodeName, caso afirmativo retira apenas a descrição dos nós
				char[] separators = {' ', '='};
				if (line.StartsWith ("NodeName")) {
					string nodesDescription = line.Split( separators, 10)[1];
					string nodesCores = line.Split(separators,10)[3];

					separators[0] = '[';
					separators[1] = ']'; 
					string prefix = nodesDescription.Split(separators,10)[0];
					string exp = nodesDescription.Split(separators,10)[1];

					// exp deve ser do formato [num1-num2,num3-num4]. Sendo que também pode ser apenas [num] ou [num1-num2] ou [num1,num2,num3]
					if (!exp.Contains(",") && !exp.Contains("-"))
						nodeList.Add(new Node(prefix + exp, int.Parse(nodesCores)));
					else if (!exp.Contains(",") && exp.Contains("-")) {
						separators[0] = '-';
						int first = int.Parse(exp.Split(separators, 10)[0]);
						int second = int.Parse(exp.Split(separators, 10)[1]);

						if (second <= first) {
							Console.WriteLine("Bad slurm.conf file");
							return;
						}

						for (int i = first; i < second + 1; i++)
							nodeList.Add(new Node(prefix + i, int.Parse(nodesCores)));
					} else if (exp.Contains(",") && !exp.Contains("-")) {
						separators[0] = ',';
						string[] nodesNumbers = exp.Split(separators, 100);
						for (int i = 0; i < nodesNumbers.Length; i++)
							nodeList.Add(new Node(prefix + nodesNumbers[i], int.Parse(nodesCores)));
					} else {
						separators[0] = ',';
						string [] exps = exp.Split(separators, 100);
						foreach (string e in exps) {
							separators[0] = '-';
							int first = int.Parse(e.Split(separators, 10)[0]);
							int second = int.Parse(e.Split(separators, 10)[1]);

							if (second <= first) {
								Console.WriteLine("Bad slurm.conf file");
								return;
							}

							for (int i = first; i < second + 1; i++)
								nodeList.Add(new Node(prefix + i, int.Parse(nodesCores)));
						}
					}
				}
			}
			// Agora que temos a lista de nós formada, vamos verificar se estão ativos. 
			List <Node> temp = new List<Node> ();
			foreach (Node node in nodeList) 
			{
				node.updateState();
				if (!node.State.Equals("DOWN*"))
					temp.Add(node);
			}
			nodeList = temp;

			fileReader.Close ();
		}

		public void updateNodesInfo ()
		{
			// Para cada nó disponível, atualizar sua carga.
			for (int i = 0; i < nodeList.Count; i++) 
			{
				nodeList [i].updateLoad ();
				nodeList [i].updateState ();
			}

			lastupdate = DateTime.Now;
		}

		public void printNodesLoad()
		{
			// Imprimir relatório de cargas.
			nodeList.Sort ();
			List<Node> weirdNodes = new List<Node> ();
			List<Node> overloadedNodes = new List<Node> ();

			Console.WriteLine ("### LOAD REPORT ###");
			for (int i = 0; i < nodeList.Count; i++) {
				Console.WriteLine ("{0}:{1:F2}", nodeList[i].Hostname, nodeList[i].Load);
				if (nodeList [i].isOverloaded ())
					overloadedNodes.Add (nodeList [i]);
				else if (!nodeList [i].State.Equals ("IDLE") && nodeList [i].Load < 2.00)
					weirdNodes.Add (nodeList [i]);
			}

			Console.WriteLine ("### OVERLOADED NODES ###");
			overloadedNodes.Sort ();
			foreach (Node n in overloadedNodes) {
				Console.WriteLine ("{0}:{1:F2}", n.Hostname, n.Load);
			}

			Console.WriteLine ("### WEIRD NODES ###");
			overloadedNodes.Sort ();
			foreach (Node n in weirdNodes) {
				Console.WriteLine ("{0}:{1:F2}", n.Hostname, n.Load);
			}
		}
	}
}

