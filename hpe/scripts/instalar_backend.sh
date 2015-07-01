#!/bin/bash
# Script para instalar o HPE BackEnd

# Variáveis que precisam ser preenchidas!
HPE_USER=
HPE_USER_PASSWD=
USER_DIR=$(grep ${HPE_USER} /etc/passwd | cut -f6 -d:);
DATABASE_USER="hashmodel";
DATABASE_PASSWORD="1566hpe";
MPI_NET_DIR="/opt/mpi.net";
MPI_NET_URL="https://github.com/jmhal/MPI.NET";
MYSQL_NET_DIR="/opt/mysql-connector-net";
MYSQL_NET_URL="https://github.com/jmhal/parallel/raw/master/hpe/mysql-connector-net-6.9.6-noinstall.zip";
BACKEND_DIR="/home/jmhal/Hash-Programming-Environment/HPE_BackEnd";
CACHE_DIR="~${HPE_USER}/hpe/cache";
WORK_DIR="~${HPE_USER}/hpe/work";
PATH_DIR="~${HPE_USER}/hpe/path";
UNIT_PATH_DIR="~${HPE_USER}/hpe/unit";

## Instalar os pacotes ##
export DEBIAN_FRONTEND=noninteractive;
apt-get update -q;

# Pacotes básicos de compilação e ambiente Mono
apt-get install -q -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" \
debconf-utils libtool automake subversion git g++ libopenmpi-dev openmpi-common openmpi-bin mono-runtime \
mono-runtime-common mono-devel mono-xsp2 mono-xsp2 mono-2.0-service mono-4.0-service; 

# Instalação do mysql-server
echo mysql-server-5.5 mysql-server/root_password password ${DATABASE_PASSWORD} | debconf-set-selections;
echo mysql-server-5.5 mysql-server/root_password_again password ${DATABASE_PASSWORD} | debconf-set-selections;
apt-get install -q -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" mysql-server;
mysql -u root -p${DATABASE_PASSWORD} <<EOF
create database hashmodel;
grant all privileges on hashmodel.* to '${DATABASE_USER}'@'%' identified by '${DATABASE_PASSWORD}';
grant all privileges on hashmodel.* to '${DATABASE_USER}'@'localhost' identified by '${DATABASE_PASSWORD}';
EOF

## Instalar MPI.NET ##
cd ${USER_DIR};
mkdir ${MPI_NET_DIR};
git clone ${MPI_NET_URL} mpi.net
cd mpi.net;
LOCAL_DIR=/opt/mpi.net;
sh autogen.sh;
./configure --prefix=${MPI_NET_DIR};
make;
make install;
gacutil -i /opt/mpi.net/lib/MPI.dll;
gacutil -i /opt/mpi.net/lib/MPIUtils.dll;

# Testando MPI e MPI.NET
cd ${USER_DIR};
wget -q https://raw.githubusercontent.com/jmhal/parallel/master/hpe/scripts/testes/TesteMPI.c;
mpicc TesteMPI.c -o TesteMPI;
test=$(mpirun -np 2 ./TesteMPI | grep OK | wc -l);
if [ $test -eq 2 ]; then
   echo "MPI OK";
else 
   echo "MPI NOT OK";
   exit 1;
fi

wget -q https://raw.githubusercontent.com/jmhal/parallel/master/hpe/scripts/testes/TesteMPINET.cs; 
dmcs -warn:0 -r:${MPI_NET_DIR}/lib/MPI.dll TesteMPINET.cs;
export LD_LIBRARY_PATH=${MPI_NET_DIR};
test=$(mpirun -np 2 mono ./TesteMPINET.exe | grep OK | wc -l);
if [ $test -eq 2 ]; then
   echo "MPI.NET OK";
else 
   echo "MPI.NET NOT OK";
   exit 1;
fi
rm -f TesteMPI TesteMPI.c TesteMPINET.*;

## Instalar o MySQL.NET ##
cd ${USER_DIR};
mkdir ${MYSQL_NET_DIR};
wget -q ${MYSQL_NET_URL};
unzip mysql-connector-net-6.9.6-noinstall.zip -d ${MYSQL_NET_DIR};
gacutil -i ${MYSQL_NET_DIR}/v2.0/MySql.Data.dll;

# Testando MySQL.NET
cd ${USER_DIR};
wget -q https://raw.githubusercontent.com/jmhal/parallel/master/hpe/scripts/testes/TesteMYSQLNET.cs;
sed -i s/usuario/${DATABASE_USER}/ TesteMYSQLNET.cs;
sed -i s/senha/${DATABASE_PASSWORD}/ TesteMYSQLNET.cs;
dmcs -r:System.Data.dll -r:/opt/mysql-connector-net/v2.0/MySql.Data.dll TesteMYSQLNET.cs;
test=$(mono TesteMYSQLNET.exe | grep schema | wc -l);
if [ $test -eq 1 ]; then
   echo "MySQL.NET OK";
else
   echo "MySQL.NET NOT OK";
   exit 1;
fi
rm -f TesteMYSQLNET.* mysql-connector-net-6.9.6-noinstall.zip;

## Compilar HPE_BackEnd ##
cd ${USER_DIR};

# Baixar o código e configurar os diretórios
git clone https://github.com/UFC-MDCC-HPC/Hash-Programming-Environment;
cd Hash-Programming-Environment/HPE_BackEnd/;
mkdir bin;

# Compilar DGAC
sed -i s/heron/${HPE_USER}/ DGAC/DGAC.cs;
sed -i s/narf@jchf9200/${HPE_USER_PASSWD}/ DGAC/DGAC.cs;
sed -i s/hash-programming-environment/Hash-Programming-Environment/ DGAC/DGAC.cs;
sed -i s/configfile/app/ DGAC/DGAC.cs;
sed -i s/worker.launch.config.9/worker.launch.config/ DGAC/DGAC.cs;
sed -i s#/usr/lib/mono/MySql.Data#${MYSQL_NET_DIR}/v2.0# make_dgac.rsp;
sed -i s#/usr/lib/mono/MPI#${MPI_NET_DIR}/lib# make_dgac.rsp;
dmcs @make_dgac.rsp;
gacutil -i bin/DGAC.dll;

# Compilar worker
sed -i s#/usr/lib/mono/MPI#${MPI_NET_DIR}/lib# make_worker.rsp;
sed -i s#/usr/lib/mono/DGAC#${BACKEND_DIR}/bin# make_worker.rsp;
dmcs @make_worker.rsp;

# Compilar manager
sed -i s#-r:DGAC#-r:${BACKEND_DIR}/bin/DGAC.dll# make_manager.rsp;
dmcs @make_manager.rsp;

# Compilar WS
sed -i s#/usr/lib/mono/DGAC#${BACKEND_DIR}/bin# make_backendws.rsp;
dmcs @make_backendws.rsp;

# Compilar run_app
sed -i s#-r:DGAC#-r:${BACKEND_DIR}/bin/DGAC.dll# make_run_app.rsp;
dmcs @make_run_app.rsp;

# Criar diretórios auxiliares 
mkdir -p ${CACHE_DIR};
mkdir -p ${WORK_DIR};
mkdir -p ${PATH_DIR};
mkdir -p ${UNIT_PATH_DIR};

# Baixar os arquivos hpe.backend.properties e external references
cd ${USER_DIR};
wget -q https://raw.githubusercontent.com/jmhal/parallel/master/hpe/scripts/hpe.backend.properties;
wget -q https://raw.githubusercontent.com/jmhal/parallel/master/hpe/scripts/externalreferences.xml;
sed -i s#WORKDIR#${WORK_DIR}# hpe.backend.properties;
sed -i s#PATHDIR#${PATH_DIR}# hpe.backend.properties;
sed -i s#UNITDIT#${UNIT_PATH_DIR}# hpe.backend.properties;
sed -i s#DGACDIR#${BACKEND_DIR}/bin/# hpe.backend.properties;
sed -i s##${USER_DIR}# hpe.backend.properties;
sed -i s#MPINETDIR#${MPI_NET_DIR}# externalreferences.xml;
