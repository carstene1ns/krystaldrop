CC=g++
LINK=$(CC)

SRC= main.cpp \
     game/set.cpp \
     game/row.cpp \
     game/hand.cpp \
     interf/Application.cpp \
     interf/Controller.cpp \
     interf/StartController.cpp \
     video/Display.cpp
     
OBJ:=$(SRC:%.cpp=%.o)
LIBS:=-L/usr/lib -lSDL -lpthread -L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv

CCFLAGS=-ggdb -DDEBUG
#CCFLAGS=-O2 -DNDEBUG
LCFLAGS=


all: drop

drop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

${OBJ}: %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
