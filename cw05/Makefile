.PHONY: clean all

clean:
	rm -f zad1 sender catcher

all:
	make zad1

zad1: zad1.c
	gcc -Wall zad1.c -o zad1
	./zad1 none


catcher: catcher.c
	gcc -Wall catcher.c -o catcher
	./catcher

sender: sender.c
	gcc -Wall sender.c -o sender

