# Instalação do HPE no Ubuntu 14.04 LTS

Este documento detalha o processo de instalação e configuração do HPE (*Hash Programming Environment*) em uma imagem recém instalada do Ubuntu 14.04 LTS 64 bits. Sempre que possível utilizamos os pacotes fornecidos pela distribuição com o objetivo de tornar esse tutorial o mais fácil possível de ser completado. Quando for necessário, informamos como e quando compilar os pacotes extras que são necessários. 
	
## 1. Compilando e Executando o Back-End ##

Esta etapa consiste na configuração do Back-End (Manager, Workers e Web Service). 

### 1. Pré-requisitos ###

Nós precisamos do Mono, do MySQL e de uma versão do MPI. A versão do Mono que usaremos é a padrão do Ubuntu. Existem várias bibliotecas MPI nos repositórios Ubuntu, nós vamos utilizar a OpenMPI. Com o Mono e o MPI instalados, vamos compilar a MPI.NET. Por fim, o MySQL se faz necessário para o banco relacional dos componentes. Para tornar a tarefa mais fácil, vamos instalar o phpmyadmin para gerenciar o banco. 

#### Instalação dos Pacotes ####

  > $ sudo apt-get install libtool automake subversion git g++ libopenmpi-dev openmpi-common openmpi-bin mono-runtime mono-runtime-common mono-devel mono-xsp2 mono-2.0-service mono-4.0-service mysql-server phpmyadmin

  Tanto para o MySQL quanto para o phpmyadmin surgem caixas de diálogos perguntando senhas para os usuários administradores. Informe a que você achar melhor. Escolha para configurar o phpmyadmin no Apache. Qualquer outra pergunta que surgir, deixe na opção default.

  #### MPI ####

  Vamos testar a instalação do OpenMPI. Copie e cole o código abaixo em arquivo chamado HelloWorldMPI.c:

  ``` c
  #include <stdio.h>
  #include <mpi.h>

  int main (int argc, char *argv[]) {
  	int rank, size;

  	MPI_Init (&argc, &argv);	
  	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
  	MPI_Comm_size (MPI_COMM_WORLD, &size);	
  	printf( "Hello world from process %d of %d\n", rank, size );
  	MPI_Finalize();
  	return 0;
  }
  ```
  
  Vamos compilar e executar:
  > $ mpicc HelloWorldMPI.c -o HelloWordMPI

  > $ mpirun -np 4 ./HelloWordMPI 

  > Hello world from process 3 of 4

  > Hello world from process 0 of 4

  > Hello world from process 1 of 4

  > Hello world from process 2 of 4

  Se o resultado for igual ao demonstrado acima, a instalação do OpenMPI está correta. 

  #### MPI.NET ####

  Precisamos também do MPI.NET. Esse precisamos compilar. Como superusuário, executamos os seguintes comandos:

  > \# mkdir /opt/mpi.net

  > \# git clone https://github.com/jmp75/MPI.NET.git mpi.net

  > \# cd mpi.net 

  > \# LOCAL_DIR=/opt/mpi.net

  > \# sh autogen.sh

  > \# ./configure –prefix=/opt/mpi.net

  > \# make

  > \# make install

  > \# gacutil -i /opt/mpi.net/lib/MPI.dll

  > \# gacutil -i /opt/mpi.net/lib/MPIUtils.dll

  Para testar, salve o seguinte código em um arquivo chamado HelloWorldMPI.cs

  ``` csharp
  using System;
  using System.Collections.Generic;
  using MPI;
 
  class Hello{
      static void Main(string[] args){
          IDictionary<int, MPI.Intracommunicator> colors = new Dictionary<int, MPI.Intracommunicator>();
  
          MPI.Environment mpi = new MPI.Environment(ref args);
          MPI.Intracommunicator worldComm = Communicator.world;
          int min = 8; int max=min; int rank = min;
 
          rank = worldComm.Rank;
          min = worldComm.Allreduce<int>(rank+1, MPI.Operation<int>.Min);
          max = worldComm.Reduce<int>(rank+1, MPI.Operation<int>.Max, 1);
 
          colors[rank] = (MPI.Intracommunicator)worldComm.Split(rank+100, rank);
 
          int rec = 8;
          if (rank == 0) {
              MPI.Request req = worldComm.ImmediateSend<int> (50, rank, 0);
              rec = worldComm.Receive<int> (rank, 0);
              req.Wait();
          }

          Console.WriteLine("Node {0} of {1} - min={2} - max={3} - rec={4}\n", rank, worldComm.Size, min,  max, rec);
          mpi.Dispose();
       }
  }

  ```

  Compile o código com:

  > $ dmcs -r:/opt/mpi.net/lib/MPI.dll HelloWorldMPI.cs

  E execute com:

  > $ export LD_LIBRARY_PATH=/opt/mpi.net/lib

  > $ mpirun -np 4 mono ./HelloWorldMPI.exe
 
  > Node 1 of 4 - min=1 - max=4 - rec=8

  > Node 2 of 4 - min=1 - max=0 - rec=8

  > Node 3 of 4 - min=1 - max=0 - rec=8

  > Node 0 of 4 - min=1 - max=0 - rec=50

  Você pode querer colocar a variável LD_LIBRARY_PATH no seu arquivo .bashrc para não ter que digitá-la toda vez. Outra opção é adicionar o diretório /opt/mpi.net/lib ao arquivo /etc/ld.so.conf. A última opção é a melhor porque vale para todos os usuários do sistema. 

  #### MySQL ####

  Agora vamos criar um banco de dados chamados "hashmodel" para armazenar informações sobre os componentes. Para facilitar a configuração, também vamos criar um usuário chamado "hashmodel". É através dele que    vamos logar no banco, seus privilégios são retristos à base "hashmodel".
  
  Entre no MySQL: 

  > $ mysql -u root -p

  Execute os comandos:	

  ``` sql
  create database hashmodel;
  grant all privileges on hashmodel.* to 'hashmodel'@'%' identified by 'senhaquedesejar';
  ```

  Em seguida, tente logar com o novo usuário:

  > $ mysql -u hashmodel -p

  Se você conseguir entrar e o comando `show databases;` mostrar o banco de dados "hashmodel", então a configuração está correta. Nós vamos preencher esse banco com um esquema na seção que descreve a Compilação do Back-End. Entretanto, para acessar o banco, o Mono precisa de um conector MySQL. A primeira coisa é baixar o conector a partir de http://dev.mysql.com/downloads/connector/net/. Escolha a plataforma .NET & Mono. Você deve baixar um arquivo com um nome semelhante à mysql-connector-net-6.9.6-noinstall.zip. Descompacte e observe que existem as pastas v2.0, v4.0 e v4.5. Por segurança, vamos instalar a versão mais antiga, v2.0. Qualquer problema, basta remover e instalar outra versão. Para instalar, entre na pasta v2.0 e execute o comando:

  > $ sudo gacutil -i MySql.Data.dll

  Depois, vamos guardar o MySql.Data.dll em um local acessível:

  > $ sudo unzip mysql-connector-net-6.9.6-noinstall.zip -d /opt/mysql-connector-net

  Vamos testar com o seguinte código no arquivo HelloWorldMySQL.cs:

  ``` csharp
  using System;
  using System.Data;
  using MySql.Data.MySqlClient;

   public class Test
   {
      public static void Main(string[] args)
      {
         string connectionString =
            "Server=localhost;" +
            "Database=hashmodel;" +
            "User ID=hashmodel;" +
            "Password=suasenha;" +
            "Pooling=false";

         IDbConnection dbcon;
         dbcon = new MySqlConnection(connectionString);
         dbcon.Open();
         IDbCommand dbcmd = dbcon.CreateCommand();

         string sql =  "show databases";
         dbcmd.CommandText = sql;
         IDataReader reader = dbcmd.ExecuteReader();
         while(reader.Read()) {
            Console.WriteLine(reader.GetString(0));
         }

         reader.Close();
         reader = null;
         dbcmd.Dispose();
         dbcmd = null;
         dbcon.Close();
         dbcon = null;
      }
   }
  
  ```

  Novamente compilamos com:

  > $ dmcs -r:System.Data.dll -r:/opt/mysql-connector-net/v2.0/MySql.Data.dll HelloWorldMySQL.cs

  E executamos:
 
  > $ mono HelloWorldMySQL.exe
  
  > information_schema

  > hashmodel

  Retornando as duas strings acima, a configuração está correta.

### 2. Compilação do Back-End ###

  A configuração do Back-End envolve fazer o *checkout* do código do repositório e compilar o código. Para fazer o checkout, basta executar:

  > $ svn checkout https://hash-programming-environment.googlecode.com/svn/trunk/HPE_BackEnd hash-programming-environment

  Esse comando vai salvar todo o projeto HPE na pasta hash-programming-environment.

  #### DGAC ####
  Vamos compilar a DGAC.dll, uma biblioteca que deve ser instalada no GAC da instalação corrente do Mono. Como o HPE é um processo em ativo desenvolvimento, precisamos fazer algumas alterações no código para executar de acordo com nosso tutorial. No arquivo DGAC/DGAC.cs, onde tiver as seguintes linhas:

  ``` csharp
  string userName = "heron";
  string password_ = "narf@jchf9200";
  ```

  Troque por o nome do seu usuário e sua senha. No mesmo arquivo, nas linhas:

  ``` csharp
  string cmd = "mpirun";
  string args = "--configfile worker.launch.config.9";
  ```
  
  No lugar de *--configfile* coloque *--app*. Essa mudança é necessária para o OpenMPI. Também troque *worker.launch.config.9* por *worker.launch.config.N*, no qual N é quantos núcleos seu processador tem. Isso Será explicado mais adiante. Precisamos também atualizar o arquivo make_dgac.rsp com a localização da biblioteca do MySQL (/opt/mysql-connector-net/v2.0/MySql.Data.dll) e do MPI (/opt/mpi.net/lib/MPI.dll). Substitua as linhas

  ```
  -r:/usr/lib/mono/MySql.Data/MySql.Data.dll
  -r:/usr/lib/mono/MPI/MPI.dll
  ```
  
  Por essas:

  ```
  -r:/opt/mysql-connector-net/v2.0/MySql.Data.dll
  -r:/opt/mpi.net/lib/MPI.dll
  ```

  Dentro do diretório hash-programming-environment, execute:

  > $ dmcs @make_dgac.rsp

  Vários *warnings* são comuns. Dentro da pasta bin/, você encontrará o arquivo DGAC.dll. Adicione ele ao GAC com o comando:

  > $ sudo gacutil -i bin/DGAC.dll

  #### Compilar o Worker, Manager, Web Service e hpe_run ####
  
  Todos esses programas são construídos de maneira semelhante ao DGAC. Novamente, o único cuidado necessário é atualizar os arquivos .rsp com a localização das bibliotecas .dll. Você precisa saber de antemão a localização da DGAC.dll, MySql.Data.dll e MPI.dll. Para facilitar, se estiver seguindo nosso tutorial à risca, a localização dessas .dlls é a seguinte:

  ```
  DGAC: /home/<seuusuario>/hash-programming-environment/bin/DGAC.dll
  MySQL: /opt/mysql-connector-net/v2.0/MySql.Data.dll
  MPI: /opt/mpi.net/lib/MPI.dll
  ```
  Faça as alterações necessárias nos arquivos e execute os comandos na pasta hash-programming-environment.

  Para o Worker, atualize as linhas do DGAC e do MPI no arquivo make_worker.rsp. 

  > $ dmcs @make_worker.rsp 

  Verifique se o arquivo WorkerService.exe foi criado na pasta bin/.
  Para o Manager, no arquivo make_manager.rsp no lugar de -r:DGAC, substitua DGAC pelo caminho da .dll.

  > $ dmcs @make_manager.rsp 

  Verifique se o arquivo ManagerService.exe foi criado na pasta bin/. Para o Web Service, no arquivo make_backendws.rsp atualize a localização do DGAC.dll.

  > $ dmcs @make_backendws.rsp 

  Verifique se a pasta ws/bin/ foi criada. Para o hpe_run, no arquivo make_run_app.rsp, no lugar de -r:DGAC, substitua DGAC pelo caminho da .dll.

  > $ dmcs @make_run_app.rsp 

  Verifique se o arquivo hpe_run.exe foi criado na pasta bin/. Novamente, vários *warnings* são emitidos. Não precisa se preocupar.

  #### Executar os serviços ####

  Com os programas compilados, vamos para a execução dos serviços.

  ##### Configuração dos Workers #####

  Copie o arquivo hpe.backend.properties para a raiz da sua pasta de usuário. Esse arquivo possui diversas informações que deve ser atualizadas de acordo com o contexto da sua instalação. Deixe o parâmetro **host** inalterado. No parâmetro **connection_string**, coloque o usuário e senha do banco de dados criado. Os parâmetros **path_temp_worker**, **path_bin** e **unit_package_path** vamos para alterar para o diretório /tmp. Em **path_dgac** colocamos o caminho para DGAC.dll. O compilador C# deve ser colocado em **cs_compiler** como /usr/bin/dmcs. Os programas para os parâmetros **gac_util**, **key_generator**, **cli_runtime** e **mpi_run** também estão na pasta /usr/bin/. Crie um arquivo chamado externalreferences.xml no seu diretório raiz do usuário. Configure o parâmetro **external_references_file** para apontar para esse novo arquivo, que deve conter o seguinte conteúdo:

  ``` xml 
  <?xml version="1.0" encoding="ASCII"?>
   <externalreferences:external xmlns:externalreferences="http://www.example.org/externalreferences">
   <externalreferences:reference description="" destailedName="MPI" path="/opt/mpi.net/lib/"/>
  </externalreferences:external>
  ```

  Crie o arquivo /etc/hpe_nodes como superusuário. Dentro dele você colocará os nomes do *hosts* e as portas do *workers*. Em geral,  um *worker* equivale a um processador ou núcleo. Por exemplo, para uma máquina de 4 núcleos, o arquivo teria essas informações:

  ```
  localhost 4865
  localhost 4866
  localhost 4867
  localhost 4868

  ```

  Se você tiver mais núcleos, basta ir incrementando o número da porta. No caso você tenha um *cluster* devidamente configurado, no lugar de *localhost*, deve colocar o *hostname* das máquinas. Em uma etapa mais adiante explicamos como adicionar mais máquinas *workers*. Por enquanto, ficamos no *localhost*. Crie em seguida um arquivo chamado worker.launch.config.N, no qual N corresponde a quantidade de *hosts* adicionados no arquivo /etc/hpe_nodes. O conteúdo de worker.launch.config.4 seria:

  ```
  -n 1 /usr/bin/mono-service -l:Worker1.lock bin/WorkerService.exe --port 4865 --debug --no-daemon
  -n 1 /usr/bin/mono-service -l:Worker2.lock bin/WorkerService.exe --port 4866 --debug --no-daemon
  -n 1 /usr/bin/mono-service -l:Worker3.lock bin/WorkerService.exe --port 4867 --debug --no-daemon
  -n 1 /usr/bin/mono-service -l:Worker4.lock bin/WorkerService.exe --port 4868 --debug --no-daemon
  ```

  O que fazemos é lançar os *workers* como uma aplicação MPMD. Em geral, o MPI trabalha com o paradigma SPMD. Cada implementação do comando *mpirun* tem sua maneira de lidar com os vários programas que fazem parte de uma aplicação MPMD. No caso do OpenMPI, o parâmetro --app é utilizado. Para para testar a execução dos *workers*:

  > $ mpirun --app worker.launch.config.4

  > Initializing Worker

  > LOADING PROPERTIES FILE: 

  > /home/azureuser/hpe.backend.properties

  > Initializing Worker

  > LOADING PROPERTIES FILE: 
 
  > /home/azureuser/hpe.backend.properties 

  > --port 4865

  > Starting MPI ... 
 
  > ...

  Vão surgir mensagens das portas para cara *worker*. Se não surgir nenhuma mensagem de erro, os *workers* estão configurados e executando. Pode finalizá-los e remover os arquivos .lock criados. Para a execução final, os *workers* são criados pelo *manager*.

  ##### Manager #####

  Apesar de não ser necessariamente relacionado ao *manager*, vamos carregar as tabelas do banco de dados. Na pasta hash-programming-environment, entre no terminal do MySQL e execute:

  > $ mysql -u hashmodel -p
 
  > mysql> use hashmodel;

  > mysql> source DGAC2.sql;

  Como é a primeira vez que instalamos o banco, ele vai reclamar de algumas tabelas inexistentes. É seguro ignorar. Agora vamos executar o *manager*:
 
  > sudo /usr/bin/mono-service2 bin/ManagerService.exe --debug --no-daemon

  Temos a saída algo parecido:

  ```
  Starting Manager 
  Manager Service ManagerHost running ...
  azureruser runs mpirun --app worker.launch.config.2 on hash-programming-environment
  OK !
  Initializing Worker
  LOADING PROPERTIES FILE: 
  /home/azureuser/hpe.backend.properties
  Initializing Worker
  LOADING PROPERTIES FILE: 
  /home/azureuser/hpe.backend.properties
  --port 4866
  Starting MPI ... 
  --port 4865
  Starting MPI ... 
  ok !ok !Rank #1 at processor jmhal - threading is Serialized
  Starting WorkerService-1 listening on port ... 4866
  Rank #0 at processor jmhal - threading is Serialized
  Starting WorkerService-0 listening on port ... 4865
  Worker #1 running !
  Worker #0 running !

  ```
 
  Devemos lembrar que o tamanho da saída varia com a quantidade de #workers#. Pode cancelar o #manager# com crtl-c. 

  ##### Web Service #####
 
  Para executar o *web service* entre na pasta ws e execute o comando:

  > $ sudo xsp2 --port 8080

  O serviço é iniciado na porta 8080. Esse é o único serviço que precisa ser inicializado, pois quando necessário, inicia o *manager*, que por sua vez inicia os *workers*. Com isso, o Back-End está configurado e pronto para executar componentes. 


## 2. Executando o Front-End ##

  Esta etapa consiste na configuração do Eclipse para executar o Front-End, ambiente no qual são construídos os componentes. O Front-End é um *plugin* que executa dentro do Eclipse. Pode parecer estranho à primeira vista, mas como é um trabalho acadêmico, o usuário executa uma versão do Eclipse e em seguida executa outro Eclipse. Em outras palavras, Eclipception. 
  Novamente, vamos fazer o *checkout* do projeto. Mas ao contrário do BackEnd, vamos baixar o projeto inteiro. Estou escrevendo esse tutorial considerando o Back-End e o Front-End em máquinas diferentes. Mas nada impede que tudo seja executado na mesma (talvez seja até melhor começar usando apenas uma máquina). 
  
  > $ svn checkout https://hash-programming-environment.googlecode.com/svn/trunk/ hash-programming-environment

  Como estou fazendo em máquinas diferentes, usei a mesma pasta. Mas você pode usar um outra pasta. Em seguida, a sugestão é usar a versão 1.7 do Java. Vamos instalar com o comando:

  > $ sudo apt-get install oracle-java7-installer

  O próximo passo é baixar uma versão do Eclipse de www.eclipse.org. Por segurança, baixe a versão Eclipse IDE for Java EE Developers. Essa versão é mais completa, a versão tradicional pode não ter algum pré-requisito. Sugiro usar essa versão do Eclipse exclusivamente para o HPE, assim como um diretório de *workspace* (workspace/workspace-hpe/) apenas para ele.
  Vamos configurar o Eclipse para usar o Java 1.7:
 
  1. Abra o Eclipse.
  2. Abra em Window -> Preferences.
  3. Vá em Java -> Installed JREs. Se já estiver instalada a java-7-oracle, marque esta opção. Caso contrário, clique em Add..., selecione Standard VM, vá em Next e coloque com JRE Home o valor /usr/lib/jvm/java-7-oracle. Depois em Java -> Compiler certifique que Compiler Compliance Level está 1.7.


  Vamos agora importar os projetos. Primeiro o HCLCompiler:
  1. File -> Import...
  2. General -> Existing Projects into Workspace
  3. Escolha como root directory o diretório hash-programming-environment/HCLCompiler
  4. Finish


  Em seguida, devemos importar o HPE_FrontEnd. São os mesmos passos do HCLCompiler. Após a importação, o projeto apresenta um erro em relação a um arquivo .xsd. Para solucionar, devemos fazer uma pequena alteração no arquivo .project na pasta hash-programming-environment/HPE_Frontend após a importação. Lembre-se, projeto acadêmico. Abra o arquivo e encontre essa parte:

  ``` xml 
  <linkedResources>
    <link>
      <name>src/hPE/frontend/connector/xml/hpe_component.xsd</name>
        <type>1</type>
	<locationURI>../src/hPE/frontend/connector/xml/hpe_component.xsd</locationURI>
     	</link>
  </linkedResources>
  ```

  E faça a pequena mudança:

  ``` xml 
  <linkedResources>
    <link>
      <name>../src/hPE/frontend/connector/xml/hpe_component.xsd</name>
        <type>1</type>
	<locationURI>src/hPE/frontend/connector/xml/hpe_component.xsd</locationURI>
     	</link>
  </linkedResources>
  ```
  
  Dê um *refresh* no projeto. Vai surgir uma mensagem de erro sobre uma pasta src/, mas depois o erro de compilação desaparecerá. Agora, precisamos criar os arquivos **hpe.frontend.properties** e **BackEndLocations.xml**. O primeiro deve ficar na pasta /home do usuário. Já o segundo na pasta /home/*usuario*/hpe. No hpe.frontend.properties, vamos preenchê-lo assim:

  ```
  backend_locations = /home/usuario/hpe/BackEndLocations.xml
  sn_path = /usr/bin/sn
  gacutil_path = /usr/bin/gacutil
  dgac_path = /*caminho do arquivo*/DGAC.dll
  cache_root = /home/usuario/hpe/cache
  mono_path = /usr/lib/mono
  mono_bin_path = /usr/bin
  mono_lib_version = 2.0
  compiler_flag_unsafe=true
  compiler_flag_debug=false
  compiler_flag_optimize=true
  ```

  Veja que precisamos do caminho do DGAC.dll. Podemos construí-lo da mesma maneira que fizemos para o Back-End. Depois basta colocar o caminho da .dll no arquivo. O arquivo BackEndLocations.xml diz para o Front-End quais são os Back-End disponíveis. Seu conteúdo é do seguinte formato:

  ``` xml 
  <?xml version="1.0" encoding="ASCII"?> 
  <locations:services xmlns:locations="http://www.example.org/BackEndLocations">
    <locations:backend name="azure" uri="http://jmhal.cloudapp.net:8080/BackEndWS.asmx"/>
    <locations:backend curdir="" name="azure" uri="http://jmhal.cloudapp.net:8080	/BackEndWS.asmx"/>
  </locations:services>

  ```
  Você deve repetir as duas linhas internas para cada novo Back-End, atualizando o campo **name** e **uri**.

  Agora, voltamos para o Eclipse. Clique com o botão direito sobre o projeto HPE_FrontEnd, escolha Run As -> Eclipse Application. É para uma instância nova do Eclipse aparecer. Se isso ocorrer, estamos no caminho certo. Feche esta instância e volte para a original. Clique no projeto HPE_FrontEnd, escolha a opção Run -> Run Configurations..., confirme o Execution Environment para java-7-oracle, depois na aba Arguments mude a opção -Xmx512m para -Xmx2048m. Em seguida, clique em Run para subir o FrontEnd novamente. Dentro da nova instância, vá em Window -> Preferences. Em HPE, coloque MONO Bin Path para /usr/bin/. Com isso, o FrontEnd pronto para uso.

## 3. Deploy e execução de Componentes *Hash* ##

Aqui vamos explicar como construir um componente simples (se é que existe isso no HPE) no Front-End, fazer o deploy no Back-End e executá-lo.


