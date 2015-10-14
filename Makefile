CC = cc
CFLAGS = -Wall # Show all reasonable warnings
LDFLAGS = 




all: server client

server: server/servercode.o
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
