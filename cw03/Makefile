CXX = gcc
CFLAGS = -Wall

.PHONY: all clean

zad1: zad1.c input.txt
	$(CXX) $(CFLAGS) zad1.c -g -o zad1
	./zad1
	diff -s input.txt output1024_reversed.txt
	diff -s input.txt output1_reversed.txt

zad2: zad2.c ./*.c ./*.txt
	$(CXX) $(CFLAGS) zad2.c -g -o zad2
	./zad2

all:
	make zad1
	make zad2

clean:
	rm -f *.o zad1 zad2
