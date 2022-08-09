CC=gcc
CFLAGS=-pthread -std=c11 -fsanitize=address -D_POSIX_C_SOURCE=200112L
#CFLAGS=-pthread -std=c11 -pedantic -Wextra -Wall -fsanitize=address -Wundef -Wpointer-arith -Wcast-align

all: web-client web-server

web-client: web-client.o request.o common.o
	$(CC) $(CFLAGS) -o web-client web-client.o request.o common.o

web-server: web-server.o common.o
	$(CC) $(CFLAGS) -o web-server web-server.o common.o

web-server.o: web-server.c common/common.h
	$(CC) $(CFLAGS) -c web-server.c

web-client.o: web-client.c common/request.h common/common.h
	$(CC) $(CFLAGS) -c web-client.c

request.o: common/request.c common/request.h
	$(CC) $(CFLAGS) -c common/request.c

common.o: common/common.c common/common.h
	$(CC) $(CFLAGS) -c common/common.c

clean:
	$(RM) web-client web-server *.o *~
