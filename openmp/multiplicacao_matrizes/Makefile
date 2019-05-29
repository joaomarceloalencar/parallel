REPODIR=../../matriz
INCLUDEDIR=$(REPODIR)/include
LIBDIR=$(REPODIR)/lib
BINDIR=./bin
SRCDIR=./src

all: main

main: 
	gcc -I$(INCLUDEDIR) -c $(SRCDIR)/main.c -o $(BINDIR)/main.o
	gcc $(BINDIR)/main.o -L$(LIBDIR) -lmatriz -o $(BINDIR)/matmultvetor

clean:
	rm $(BINDIR)/*
