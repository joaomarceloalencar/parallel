using System;
using System.Diagnostics;

namespace HostTopProcesses
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			// First parameter: name of the machine
			// Second parameter: number of processes
			String hostname = args [0];
			int nproc = int.Parse (args [1]);
			Console.WriteLine ("Host: " + hostname + "  Processes: " + nproc);

			// Retrieve the most consuming processes by CPU
			var proc = new Process {
				StartInfo = new ProcessStartInfo {
					FileName = "pdsh",
					Arguments = " -w " + hostname + " ps -e --sort=-pcpu ",
					UseShellExecute = false,
					RedirectStandardOutput = true,
					CreateNoWindow = true
				}
			};

			// Return the processes name.
			proc.Start();
			while (!proc.StandardOutput.EndOfStream) {
				// Discard the first header line
				string line = proc.StandardOutput.ReadLine();

				for (int i = 0; !proc.StandardOutput.EndOfStream && (i < nproc); i++) {
					line = proc.StandardOutput.ReadLine();
				    char[] separators = {':'};
					string process = line.Split(separators,10)[3].Split(null)[1];
					Console.WriteLine (process);
     			}
				break;
			}
		}
	}
}
