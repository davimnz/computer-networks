CC=gcc
CFLAGS=-pthread -std=c11 -fsanitize=address
#CFLAGS=-pthread -std=c11 -pedantic -Wextra -Wall -fsanitize=address -Wundef -Wpointer-arith -Wcast-align

all: web-client

web-client: web-client.o request.o
	$(CC) $(CFLAGS) -o web-client web-client.o request.o

web-client.o: web-client.c common/request.h
	$(CC) $(CFLAGS) -c web-client.c

request.o: common/request.c common/request.h
	$(CC) $(CFLAGS) -c common/request.c

clean:
	$(RM) web-client *.o *~
