all: generateMatrices matMultSerial matMultOpenMP matMultPthreads

generateMatrices: libmatrix.so
	gcc -L${PWD} -Wall -o generateMatrices generateMatrices.c -lmatrix

matMultSerial: libmatrix.so
	gcc -L${PWD} -Wall -o matMultSerial matMultSerial.c -lmatrix

matMultOpenMP: libmatrix.so
	gcc -L${PWD} -Wall -o matMultOpenMP matMultOpenMP.c -lmatrix -fopenmp

matMultPthreads: libmatrix.so
	gcc -L${PWD} -Wall -o matMultPthreads matMultPthreads.c -lmatrix -pthread

libmatrix.so: matrix.o
	gcc -shared -o libmatrix.so matrix.o

matrix.o: matrix.c
	gcc -c -Wall -Werror -fpic matrix.c

clean: 
	rm -f libmatrix.so matrix.o generateMatrices matMultSerial matMultOpenMP matMultPthreads *.mtx

