# Part of this makefile is from the C'Nedra project

export

VERSION=0_3

CC=g++
LINK=gcc
MAKE=make

SRC= main.cpp           \
     names.cpp          \
     game/anim_row.cpp  \
     game/highscores.cpp \
     game/set.cpp       \
     game/memo.cpp      \
     game/parameter.cpp \
     game/row.cpp       \
     game/hand.cpp      \
     game/table.cpp	\
     interf/Application.cpp     \
     interf/CharSelectController.cpp \
     interf/Controller.cpp      \
     interf/event.cpp           \
     interf/eventmanager.cpp    \
     interf/keyboard.cpp        \
     interf/DuelController.cpp  \
     interf/HighScoresController.cpp \
     interf/StartController.cpp \
     interf/SurvivalController.cpp	\
     interf/TitleController.cpp \
     sound/music.cpp    \
     sound/sound.cpp    \
     sound/soundsystem.cpp\
     util/logfile.cpp   \
     util/direct.cpp    \
     util/textfile.cpp	\
     video/background.cpp \
     video/Display.cpp  \
     video/textevent.cpp\
     video/gem.cpp      \
     video/image.cpp	\
     video/imagemanager.cpp	\
     video/inputbox.cpp         \
     video/sprite.cpp   \
     video/spriteinstance.cpp	\
     video/font.cpp	\
     video/SDL_rotozoom.cpp

OBJ:=$(SRC:%.cpp=%.o)
DEP:=$(OBJ:%.o=dep/%.d)
LIBS:= -lefence
LIBS:= $(LIBS) -L/usr/lib -lSDL -lSDL_image -lpthread -L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv
LIBS:= $(LIBS) -lSDL_mixer

CCFLAGS=-ggdb -Wall -DDEBUG -DNO_MUSIC -DDEBUG_SANITY_CHECK
#CCFLAGS=-O2 -DNDEBUG -Wall

DCFLAGS=-MM
LCFLAGS=-lstdc++

all: drop

FORCE:

dep: $(DEP)

$(DEP): %.d: FORCE
	@$(MAKE) -s --no-print-directory -f Makefile.dep $@

drop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

$(OBJ): %.o: dep/%.d
$(OBJ): %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f drop *.o */*.o
	rm -fR dep
	rm -f log.txt

doc: FORCE
	doxygen krystal.dox

distclean: clean
	rm -fR doc/html

pack: packages
packages: package_src package_art package_bin
package_art:
	@cd art
	@rm -fR .xvpics
	@cd ..
	mkdir -p packs
	@rm -f packs/art_$(VERSION).tgz
	tar -cvzf packs/art_$(VERSION).tgz art/*.acc art/Slapstick.* art/*.ogg art/*.wav

package_src:
	@make clean
	@mkdir -p packs
	@rm -f packs/src_$(VERSION).tgz
	tar -cvzf packs/src_$(VERSION).tgz *.cpp *.h *.txt Makefile* README COPYING game/ interf/ socket/ sound/ util/ video/ VisualC/

package_bin: clean drop
	@rm -f packs/drop_lin_$(VERSION).tgz
	@mkdir -p packs
	tar -cvzf packs/drop_lin_$(VERSION).tgz README COPYING drop table*

