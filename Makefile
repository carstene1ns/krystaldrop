CC=g++
LINK=$(CC)

SRC= main.cpp           \
     game/set.cpp       \
     game/parameter.cpp \
     game/row.cpp       \
     game/hand.cpp      \
     interf/Application.cpp     \
     interf/Controller.cpp      \
     interf/StartController.cpp \
     util/logfile.cpp   \
     util/direct.cpp    \
     video/Display.cpp  \
     video/sprite.cpp   \
     video/spriteinstance.cpp

OBJ:=$(SRC:%.cpp=%.o)
LIBS:=-L/usr/lib -lSDL -lSDL_image -lpthread -L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv

CCFLAGS=-ggdb -DDEBUG -Wall
#CCFLAGS=-O2 -DNDEBUG -Wall
LCFLAGS=


all: drop

drop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

${OBJ}: %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
