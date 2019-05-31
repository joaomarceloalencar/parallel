INCLUDEDIR=./include
LIBDIR=./lib
BINDIR=./bin
SRCDIR=./src

all: main

matriz: $(SRCDIR)/matriz.c $(INCLUDEDIR)/matriz.h
	gcc -I$(INCLUDEDIR) -c $(SRCDIR)/matriz.c -fPIC -o $(BINDIR)/matriz.o
	gcc -shared $(BINDIR)/matriz.o -o $(LIBDIR)/libmatriz.so

main: matriz
	gcc -I$(INCLUDEDIR) -c $(SRCDIR)/main.c -o $(BINDIR)/main.o
	gcc $(BINDIR)/main.o -L$(LIBDIR) -lmatriz -o $(BINDIR)/matmult
	gcc $(SRCDIR)/comparaMatrizes.c -o $(BINDIR)/comparaMatrizes
	gcc $(SRCDIR)/criarMatriz.c -o $(BINDIR)/criarMatriz

blas: matriz
	gcc -I$(INCLUDEDIR) -c $(SRCDIR)/main_blas.c -o $(BINDIR)/main_blas.o
	gcc $(BINDIR)/main_blas.o -L$(LIBDIR) -lmatriz -lopenblas -o $(BINDIR)/matmult_blas

openmp: matriz
	gcc -fopenmp -I$(INCLUDEDIR) -c $(SRCDIR)/main.c -o $(BINDIR)/main_openmp.o
	gcc -fopenmp $(BINDIR)/main_openmp.o -L$(LIBDIR) -lmatriz -lopenblas -o $(BINDIR)/matmult_openmp

clean:
	rm $(LIBDIR)/*.so
	rm $(BINDIR)/*
