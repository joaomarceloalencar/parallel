using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace SlurmMonitor
{
	public class ResourceManager
	{
		private List<Node> nodeList;
		private DateTime lastupdate = new DateTime(2015, 1, 1, 0, 0, 0);

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
			updateNodesInfo ();
		}

		private List<String> nodesNames(string nodesDescription)
		{ 
			// nodesDescription é uma string no formato prefix[num1-num2,num3-num4,...], na qual o '-' representa uma faixa de valores
			List<String> nodesNames = new List<String> ();

			char [] separators = {'[',']'};
			string prefix = nodesDescription.Split(separators,10)[0];
			string exp = nodesDescription.Split(separators,10)[1];

			// exp deve ser do formato [num1-num2,num3-num4]. Sendo que também pode ser apenas [num] ou [num1-num2] ou [num1,num2,num3]
			if (!exp.Contains(",") && !exp.Contains("-"))
				nodesNames.Add(prefix + exp);
			else if (!exp.Contains(",") && exp.Contains("-")) {
				separators[0] = '-';
				int first = int.Parse(exp.Split(separators, 10)[0]);
				int second = int.Parse(exp.Split(separators, 10)[1]);

				if (second <= first) {
					Console.WriteLine("Bad slurm.conf file");
					return nodesNames;
				}

				for (int i = first; i < second + 1; i++)
					nodesNames.Add(prefix + i);
			} else if (exp.Contains(",") && !exp.Contains("-")) {
				separators[0] = ',';
				string[] nodesNumbers = exp.Split(separators, 100);
				for (int i = 0; i < nodesNumbers.Length; i++)
					nodesNames.Add(prefix + nodesNumbers[i]);
			} else {
				separators[0] = ',';
				string [] exps = exp.Split(separators, 100);
				foreach (string e in exps) {
					separators[0] = '-';
					int first = int.Parse(e.Split(separators, 10)[0]);
					int second = int.Parse(e.Split(separators, 10)[1]);

					if (second <= first) {
						Console.WriteLine("Bad slurm.conf file");
						return nodesNames;
					}

					for (int i = first; i < second + 1; i++)
						nodesNames.Add(prefix + i);
				}
			}

			return nodesNames;
		}

		// Recuperar o nome dos nós que estão ativos e adicionar numa nova lista e depois retorná-la.
		public void retrieveNodes ()
		{
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
					string nodesCores = line.Split(separators,10)[3];
					string nodesDescription = line.Split( separators, 10)[1];

					// Adiciona na lista o nome dos nós encontrados. 
					foreach (string node in nodesNames(nodesDescription))
						nodeList.Add(new Node(node, int.Parse(nodesCores)));	
				}
			}

			fileReader.Close ();
		}

		public void updateNodesInfo ()
		{
			// Verificar a ultima atualização. Para não sobrecarregar, atualizações só são após 15 minutos da última.
			TimeSpan span = DateTime.Now - lastupdate;
			if (span.Minutes < 15)
				return;

			// Para cada nó disponível, atualizar sua carga.
			foreach (Node n in nodeList) 
			{
				n.updateState ();
				if (!n.State.Equals ("DOWN*"))
					n.updateLoad ();
			}

			// Atualizar o momento da última atualização.
			lastupdate = DateTime.Now;
		}

		public void printNodesLoad()
		{
			// Imprimir relatório de cargas.
			nodeList.Sort ();
			List<Node> weirdNodes = new List<Node> ();
			List<Node> overloadedNodes = new List<Node> ();

			// Todos os nós.
			Console.WriteLine ("### LOAD REPORT ###");
			foreach (Node n in nodeList) 
			{
				if (n.State.Equals ("DOWN*"))
					continue;
				Console.WriteLine ("{0}:{1:F2}", n.Hostname, n.Load);
				if (n.isOverloaded ())
					overloadedNodes.Add (n);
				else if (!n.State.Equals ("IDLE") && n.Load < 2.00)
					weirdNodes.Add (n);
			}

			// Nós sobrecarregados.
			Console.WriteLine ("### OVERLOADED NODES ###");
			overloadedNodes.Sort ();
			foreach (Node n in overloadedNodes) {
				Console.WriteLine ("{0}:{1:F2}", n.Hostname, n.Load);
			}

			// Nós estranhos.
			Console.WriteLine ("### WEIRD NODES ###");
			overloadedNodes.Sort ();
			foreach (Node n in weirdNodes) {
				Console.WriteLine ("{0}:{1:F2}", n.Hostname, n.Load);
			}
		}
	}
}

