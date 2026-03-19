CCFLAGS:=-Wall $(shell sdl-config --cflags) -ggdb -DDEBUG 
LDFLAGS:=$(shell sdl-config --libs) -lSDL_net
OBJS:=main.o ../KDpp/Tools/Logfile.o ../KDp2p/libkdp2p.so
test:
	g++ $(CCFLAGS) -c main.cpp -o main.o
	g++ $(LDFLAGS) $(OBJS) -o p2ptest

