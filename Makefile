# Part of this makefile is from the C'Nedra project

export

CC=g++
LINK=$(CC)
MAKE=make

SRC= main.cpp           \
     game/anim_row.cpp  \
     game/set.cpp       \
     game/memo.cpp      \
     game/parameter.cpp \
     game/row.cpp       \
     game/hand.cpp      \
     game/table.cpp	\
     interf/Application.cpp     \
     interf/Controller.cpp      \
     interf/DuelController.cpp  \
     interf/StartController.cpp \
     interf/SurvivalController.cpp	\
     interf/TitleController.cpp \
     util/logfile.cpp   \
     util/direct.cpp    \
     util/textfile.cpp	\
     video/Display.cpp  \
     video/gem.cpp      \
     video/image.cpp	\
     video/imagemanager.cpp	\
     video/sprite.cpp   \
     video/spriteinstance.cpp	\
     video/font.cpp	\
     video/SDL_rotozoom.cpp

OBJ:=$(SRC:%.cpp=%.o)
DEP:=$(OBJ:%.o=dep/%.d)
LIBS:=-L/usr/lib -lSDL -lSDL_image -lpthread -L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv

CCFLAGS=-ggdb -DDEBUG -Wall
#CCFLAGS=-O2 -DNDEBUG -Wall
DCFLAGS=-MM
LCFLAGS=

all: drop

FORCE:

dep: $(DEP)



$(DEP): # %.d: FORCE
	@$(MAKE) -s --no-print-directory -f Makefile.dep $@

drop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

${OBJ}: %.o: dep/%.d
${OBJ}: %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
	rm -fR dep

