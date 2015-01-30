using System;


class WebServerClient
{
	public static void Main(string[] args) {
		if (args.Length == 1) {
			string word = args [0];
			HelloWorld webservice = new HelloWorld ();
			string res = webservice.helloWorld (word);
			Console.WriteLine (res);
		}
	}
}