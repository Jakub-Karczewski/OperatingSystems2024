.PHONY: clean

clean:
	rm -f server client 

server: server.c 
	gcc -lpthread server.c -o server
	./server

client: client.c 
	gcc -lpthread client.c -o client
	./client