.PHONY: clean del_potoki

clean:
	rm -f  zad1 sender catcher
 
del_potok:
	rm -f potok1

zad1: zad1.c 
	gcc -Wall zad1.c -o zad1
	./zad1 0.000000001 1000

sender: zad2_sender.c 
	gcc -Wall zad2_sender.c -o sender
	./sender

catcher: zad2_catcher.c
	gcc zad2_catcher.c -o catcher
	./catcher

potok:
	mkfifo -m 777 potok1
