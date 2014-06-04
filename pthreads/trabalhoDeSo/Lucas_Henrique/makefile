LIBS = -lpthread

all: main
main: main.o list.o tleitura.o queue.o thread.o
	g++ -o main main.o list.o tleitura.o queue.o thread.o $(LIBS)
main.o: main.cpp list.h thread.h
	g++ -o main.o -c main.cpp
list.o: list.cpp queue.h node.h
	g++ -o list.o -c list.cpp
tleitura.o: tleitura.cpp thread.h
	g++ -o tleitura.o -c tleitura.cpp
queue.o: queue.cpp node.h
	g++ -o queue.o -c queue.cpp
thread.o: thread.cpp
	g++ -o thread.o -c thread.cpp
clean:
	rm -rf *.o