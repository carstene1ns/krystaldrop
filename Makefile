CC=gcc
LINK=ld

SRC= main.cpp \
     game/set.cpp \
     game/row.cpp \
     game/hand.cpp \
     interf/Application.cpp
     
OBJ:=$(SRC:%.cpp=%.o)
LIBs= 

CCFLAGS=-ggdb -DDEBUG 
#CCFLAGS=-O2 -DNDEBUG
LCFLAGS=


all: drop

drop: $(OBJ)
	$(LINK) $(LCFLAGS) -o $@ $(OBJ) $(LIBS)

${OBJ}: %.o: %.cpp
	gcc $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
