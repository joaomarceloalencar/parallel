using System;
using br.ufc.pargo.hpe.backend.DGAC;
using br.ufc.pargo.hpe.basic;
using br.ufc.pargo.hpe.kinds;
using br.ufc.mdcc.HelloWorld;

namespace br.ufc.mdcc.HelloWorldImpl { 

public class IHelloWorldImpl : BaseIHelloWorldImpl, IHelloWorld
{

public IHelloWorldImpl() { 

} 

public override void main() { 
			Console.WriteLine ("O início do fim. Meu rank: " + Rank);

}

}

}
