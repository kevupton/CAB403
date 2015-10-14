CC = cc
CFLAGS = -Wall # Show all reasonable warnings
LDFLAGS = 


all: hello

hello: main.o factorial.o hello.o
    g++ main.o factorial.o hello.o -o hello

main.o: main.cpp
    g++ -c main.cpp

factorial.o: factorial.cpp
    g++ -c factorial.cpp

hello.o: hello.cpp
    g++ -c hello.cpp

clean:
    rm *o hello



all: server client

server: Server/main.o
	${CC} -o server/server server/servercode.o

servercode.o: server/servercode.c
#	${CC} -c server/servercode.c


client: client/clientcode.o
	${CC} -o client/client client/clientcode.o

clientcode.o: client/clientcode.c



clean:
	rm -f server/*.o server/server 
	rm -f client/*.o  client/client
 
.PHONY: clean all
