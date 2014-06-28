CC=mpicc
CFLAGS= -c -Wall

all: broadcast

broadcast: BroadcastAllGather.o HypercubeBroadcast.o Hypercube.o
	$(CC) BroadcastAllGather.o -o BroadcastAllGather.bin
	$(CC) HypercubeBroadcast.o Hypercube.o -o HypercubeBroadcast.bin -lm
	rm -rf *.o

BroadcastAllGather.o: BroadcastAllGather/src/BroadcastAllGather.c
	$(CC) $(CFLAGS) BroadcastAllGather/src/BroadcastAllGather.c

HypercubeBroadcast.o: HypercubeBroadcast/src/HypercubeBroadcast.c
	$(CC) $(CFLAGS) HypercubeBroadcast/src/HypercubeBroadcast.c
        
Hypercube.o: HypercubeBroadcast/src/Hypercube.c
	$(CC) $(CFLAGS) HypercubeBroadcast/src/Hypercube.c

clean:
	rm -rf *.o *.bin
