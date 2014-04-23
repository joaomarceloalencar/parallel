CC = gcc
CFLAGS = -Wall
DEPS = linked_list.h
OBJ = linked_list.o parallel_insertion.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

parallel_insertion: $(OBJ)
	gcc $(CFLAGS) -o $@ $^
