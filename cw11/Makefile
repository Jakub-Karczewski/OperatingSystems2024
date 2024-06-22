.PHONY: clean

clean:
	rm -f server client

client: client.c
	gcc -D_DEFAULT_SOURCE -lpthread client.c -o client
	./client 3000 1

server: server.c
	gcc -D_DEFAULT_SOURCE -lpthread server.c -o server
	./server 3000 

runc:
	./client 3000 1

runs:
	./server 3000
