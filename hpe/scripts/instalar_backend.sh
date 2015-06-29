#!/bin/bash
# Script para instalar o HPE BackEnd

# Variáveis 
HPE_USER=
HPE_USER_PASSWD=
DATABASE_USER="hashmodel";
DATABASE_PASSWORD="1566hpe";
MPI_NET_DIR=/opt/mpi.net/lib
MPI_NET_URL=
MYSQL_NET_DIR=/opt/mysql-connector-net/v2.0
MYSQL_NET_URL=
BACKEND_DIR="/home/jmhal/hash-programming-environment/HPE_BackEnd";
CACHE_DIR="/home/jmhal/hpe/cache";
WORK_DIR="/home/jmhal/hpe/work";
PATH_DIR="/home/jmhal/hpe/path";
UNIT_PATH_DIR="/home/jmhal/hpe/unit";

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

## Compilar HPE_BackEnd ##
cd ~${HPE_USER};

# Baixar o código e configurar os diretórios
git clone https://github.com/UFC-MDCC-HPC/Hash-Programming-Environment;
cd Hash-Programming-Environment/HPE_BackEnd/;
mkdir bin;

# Compilar DGAC
sed -i s/heron/${HPE_USER}/ DGAC/DGAC.cs;
sed -i s/narf@jchf9200/${HPE_USER_PASSWD}/ DGAC/DGAC.cs;
sed -i s/configfile/app/ DGAC/DGAC.cs;
sed -i s/worker.launch.config.9/worker.launch.config/ DGAC/DGAC.cs;
sed -i s#/usr/lib/mono/MySql.Data#${MYSQL_NET_DIR}# make_dgac.rsp;
sed -i s#/usr/lib/mono/MPI#${MPI_NET_DIR}# make_dgac.rsp;
dmcs @make_dgac.rsp;
gacutil -i bin/DGAC.dll;

# Compilar worker
sed -i s#/usr/lib/mono/MPI#${MPI_NET_DIR}# make_worker.rsp;
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

# Apagar o Banco de Dados e recarregar o esquema
#mysql -u ${DATABASE_USER} -p${DATABASE_PASSWORD} hashmodel<<EOF
#source ${BACKEND_DIR}/DGAC2.sql;
#EOF

# Compilar DGAC.dll
#cd ${BACKEND_DIR};
#gmcs @make_dgac.rsp;
#echo "Informe a senha para instalação do DGACL.dll no GAC";
#sudo gacutil -u DGAC;
#sudo gacutil -i bin/DGAC.dll;

# Compilar Manager
#cd ${BACKEND_DIR};
#gmcs @make_manager.rsp

# Compilar Worker
#cd ${BACKEND_DIR};
#gmcs @make_worker.rsp 

# Compilar WS
#cd ${BACKEND_DIR};
#gmcs @make_backendws.rsp 

# Compular o hpe_run
#cd ${BACKEND_DIR};
#gmcs @make_run_app.rsp 

# Limpar diretórios para novo deploy
#echo "Forneça senha para apagar os diretórios.";
#sudo rm -rf ${CACHE_DIR}/*;
#sudo rm -rf ${WORK_DIR}/*;
#sudo rm -rf ${PATH_DIR}/*;
#sudo rm -rf ${UNIT_PATH_DIR}/*;


