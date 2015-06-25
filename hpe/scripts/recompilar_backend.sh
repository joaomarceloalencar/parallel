#!/bin/bash
# Script para recompilar o HPE BackEnd

# Variáveis 
DATABASE_USER="hashmodel";
DATABASE_PASSWORD="1566hpe";
BACKEND_DIR="/home/jmhal/hash-programming-environment/HPE_BackEnd";
CACHE_DIR="/home/jmhal/hpe/cache";
WORK_DIR="/home/jmhal/hpe/work";
PATH_DIR="/home/jmhal/hpe/path";
UNIT_PATH_DIR="/home/jmhal/hpe/unit";

# Apagar o Banco de Dados e recarregar o esquema
mysql -u ${DATABASE_USER} -p${DATABASE_PASSWORD} hashmodel<<EOF
source ${BACKEND_DIR}/DGAC2.sql;
EOF

# Compilar DGAC.dll
cd ${BACKEND_DIR};
gmcs @make_dgac.rsp;
echo "Informe a senha para instalação do DGACL.dll no GAC";
sudo gacutil -i bin/DGAC.dll

# Compilar Manager
cd ${BACKEND_DIR};
gmcs @make_manager.rsp

# Compilar Worker
cd ${BACKEND_DIR};
gmcs @make_worker.rsp 

# Compilar WS
cd ${BACKEND_DIR};
gmcs @make_backendws.rsp 


# Limpar diretórios para novo deploy
rm -r ${CACHE_DIR}/*;
rm -r ${WORK_DIR}/*;
rm -r ${PATH_DIR}/*;
rm -r ${UNIT_PATH_DIR}/*;


