CC = gcc
CFLAGS = -Wall

all: client server clean

client, server: client.o server.o
	$(CC) $(CFLAGS) -o client client.o
	$(CC) $(CFLAGS) -o server server.o

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f *.o

