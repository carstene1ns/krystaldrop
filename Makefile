# Part of this makefile is from the C'Nedra project

export

CC=g++
LINK=gcc
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
     interf/event.cpp           \
     interf/eventmanager.cpp    \
     interf/StartController.cpp \
     interf/SurvivalController.cpp	\
     interf/TitleController.cpp \
     sound/music.cpp    \
     sound/sound.cpp    \
     sound/soundsystem.cpp\
     util/logfile.cpp   \
     util/direct.cpp    \
     util/textfile.cpp	\
     video/Display.cpp  \
     video/textevent.cpp\
     video/gem.cpp      \
     video/image.cpp	\
     video/imagemanager.cpp	\
     video/sprite.cpp   \
     video/spriteinstance.cpp	\
     video/font.cpp	\
     video/SDL_rotozoom.cpp

OBJ:=$(SRC:%.cpp=%.o)
DEP:=$(OBJ:%.o=dep/%.d)
LIBS:=-lefence -L/usr/lib -lSDL -lSDL_image -lSDL_mixer -lpthread -L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv

CCFLAGS=-ggdb -DDEBUG -Wall
#CCFLAGS=-O2 -DNDEBUG -Wall 
#CCFLAGS=-O3 -finline-functions -fstrength-reduce -fthread-jumps -fexpensive-optimizations -DNDEBUG -Wall

DCFLAGS=-MM
LCFLAGS=-lstdc++

all: drop

FORCE:

dep: $(DEP)



$(DEP): # %.d: FORCE
	@$(MAKE) -s --no-print-directory -f Makefile.dep $@

drop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

$(OBJ): %.o: dep/%.d
$(OBJ): %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
	rm -fR dep

