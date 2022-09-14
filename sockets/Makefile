CC = g++
CXXFLAGS = -std=c++0x -Wall -pthread -g -02

SRCDIR = src
OBJDIR = obj
BINDIR = bin

all: mkdirs server client

# server
server: server_main.o server_class.o common.o
	$(CC) $(CFLAGS) -o $(BINDIR)/server $(OBJDIR)/server_main.o $(OBJDIR)/server_class.o $(OBJDIR)/common.o

server_main.o: $(SRCDIR)/server/main.cpp
	$(CC) $(CFLAGS) -o $(OBJDIR)/server_main.o -c $(SRCDIR)/server/main.cpp

server_class.o: $(SRCDIR)/server/server.cpp $(SRCDIR)/server/server.h 
	$(CC) $(CFLAGS) -o $(OBJDIR)/server_class.o -c $(SRCDIR)/server/server.cpp


# client
client: client_main.o client_class.o common.o
	$(CC) $(CFLAGS) -o $(BINDIR)/client $(OBJDIR)/client_main.o $(OBJDIR)/client_class.o $(OBJDIR)/common.o

client_main.o: $(SRCDIR)/client/main.cpp
	$(CC) $(CFLAGS) -o $(OBJDIR)/client_main.o -c $(SRCDIR)/client/main.cpp

client_class.o: $(SRCDIR)/client/client.cpp $(SRCDIR)/client/client.h 
	$(CC) $(CFLAGS) -o $(OBJDIR)/client_class.o -c $(SRCDIR)/client/client.cpp

# common
common.o: $(SRCDIR)/common/common.cpp $(SRCDIR)/common/common.h
	$(CC) $(CFLAGS) -o $(OBJDIR)/common.o -c $(SRCDIR)/common/common.cpp


# utility
mkdirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

clean:
	$(RM) $(BINDIR)/* $(OBJDIR)/*
