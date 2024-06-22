.PHONY: clean
test: test.c 
	gcc test.c -lrt -lpthread -o test
	./test

clean:
	rm -f test zaj zaj1 zaj2 printer user

user: user.c 
	gcc user.c -lrt -lpthread -o user 


zaj: zajecia.c
	gcc zajecia.c -o zaj
	./zaj

zaj1: zajecia1.c
	gcc zajecia1.c -o zaj1 
	./zaj1

zaj2: zajecia2.c 
	gcc zajecia2.c -o zaj2 
	./zaj2


printer: printer.c 
	gcc printer.c -lrt -lpthread -o printer 

