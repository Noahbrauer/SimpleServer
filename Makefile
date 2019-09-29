CFLAGS = -std=c++11 -Wall

SERVER = smalld
CLIENTS = smallSet smallGet smallDigest smallRun

all: $(SERVER) $(CLIENTS)

csapp.h:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/include/csapp.h

csapp.c:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/src/csapp.c

csapp.o: csapp.h csapp.c

clients: smallSet smallGet smallDigest smallRun

smalld: smalld.cpp csapp.o
	g++ $(CFLAGS) smalld.cpp csapp.o -lpthread -o smalld

smallSet: smallSet.cpp csapp.o
	g++ $(CFLAGS) smallSet.cpp csapp.o -lpthread -o smallSet

smallGet: smallGet.cpp csapp.o
	g++ $(CFLAGS) smallGet.cpp csapp.o -lpthread -o smallGet

smallDigest: smallDigest.cpp csapp.o
	g++ $(CFLAGS) smallDigest.cpp csapp.o -lpthread -o smallDigest

smallRun: smallRun.cpp csapp.o
	g++ $(CFLAGS) smallRun.cpp csapp.o -lpthread -o smallRun

.PHONY: clean
	rm -f csapp.h csapp.c $(SERVER) $(CLIENTS)
