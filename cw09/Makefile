CC = gcc
CFLAGS = -Wall

n = 22
height = 24
width = 24
time_wait = 500


all: life
	./life $(n) $(height) $(width) $(time_wait)
	
life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncursesw -lpthread
	
clean:
	rm -f life
