CXX = gcc
CFLAGS = -Wall

.PHONY : clean all run

clean:
	rm -f *.o *.so *.so.1 *.a static_main shared_main stringlibrary a.out
	rm -f static_client dynamic_client shared_client

all: 
	make clean
	make shared_client
	make static_client
	make dynamic_client
	make objdump_static
	make objdump_shared
	make objdump_dynamic

run:
	./static_client
	./shared_client
	./dynamic_client

objdump_static: static_client
	objdump -d static_client > objdump_static.txt

objdump_shared: shared_client
	objdump -d shared_client > objdump_shared.txt

objdump_dynamic: dynamic_client
	objdump -d dynamic_client > objdump_dynamic.txt

libcollatz_shared.so: collatz.c collatz.h
	$(CXX) $(CFLAGS) -fPIC -Wl,--export-dynamic -c collatz.c
	$(CXX) $(CFLAGS) -shared -Wl,--export-dynamic -o libcollatz_shared.so collatz.o

libcollatz_static.a: collatz.c collatz.h
	$(CXX) $(CFLAGS) -c collatz.c
	ar rcs libcollatz_static.a collatz.o

shared_client: client.c libcollatz_shared.so
	$(CXX) $(CFLAGS) client.c -lcollatz_shared -L ./ -Wl,-rpath,. -o shared_client

static_client: client.c libcollatz_static.a
	$(CXX) $(CFLAGS) -c client.c
	$(CXX) $(CFLAGS) client.o libcollatz_static.a -o static_client

dynamic_client: client.c
	$(CXX) $(CFLAGS) client.c -o dynamic_client






	

