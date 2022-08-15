CC=gcc
CFLAGS=-pthread -std=c11 -fsanitize=address -D_POSIX_C_SOURCE=200112L
#CFLAGS=-pthread -std=c11 -pedantic -Wextra -Wall -fsanitize=address -Wundef -Wpointer-arith -Wcast-align

BINDIR=bin
OBJDIR=obj

all: server client

server: server.o common.o
	$(CC) $(CFLAGS) $(OBJDIR)/server.o $(OBJDIR)/common.o  -o $(BINDIR)/server

server.o: server/server.c common/common.h
	$(CC) $(CFLAGS) -c server/server.c -o $(OBJDIR)/server.o

client: client.o common.o request.o
	$(CC) $(CFLAGS) $(OBJDIR)/client.o $(OBJDIR)/common.o $(OBJDIR)/request.o  -o $(BINDIR)/client

client.o: client/client.c common/common.h common/request.h
	$(CC) $(CFLAGS) -c client/client.c -o $(OBJDIR)/client.o


common.o: common/common.c common/common.h
	$(CC) $(CFLAGS) -c common/common.c -o $(OBJDIR)/common.o

request.o: common/request.c common/request.h
	$(CC) $(CFLAGS) -c common/request.c -o $(OBJDIR)/request.o


clean:
	$(RM) $(BINDIR)/* $(OBJDIR)/*
