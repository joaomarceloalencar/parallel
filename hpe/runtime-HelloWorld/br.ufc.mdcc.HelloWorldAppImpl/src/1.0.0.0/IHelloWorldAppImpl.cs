using System;
using br.ufc.pargo.hpe.backend.DGAC;
using br.ufc.pargo.hpe.basic;
using br.ufc.pargo.hpe.kinds;
using br.ufc.mdcc.HelloWorldApp;

namespace br.ufc.mdcc.HelloWorldAppImpl { 

public class IHelloWorldAppImpl : BaseIHelloWorldAppImpl, IHelloWorldApp
{

public IHelloWorldAppImpl() { 

} 

public override void main() { 
			Helloworld.go ();

}

}

}
