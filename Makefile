# Makefile configuration
# values must be 'yes' or 'no'

# OpenGL support ? Speed up a lot the game, and allow some special effect
USE_OPENGL=yes

# Link with the SDL_mixer library ? If set to no, then no music
# nor sounds will be played.
USE_SDLMIXER=no

# If USE_SDLMIXER=yes, compile with OGG music support ?
USE_MUSIC=no

# If USE_SDLMIXER=yes, compile with WAV sound + OGG music support ?
# USE_SOUND=no implies USE_MUSIC=no
USE_SOUND=yes

# Debugging switches
# Link with efence ?
USE_EFENCE=no

# Compile with GDB debugging extensions ?
DEBUG_MODE=yes

# ######################

export
VERSION=0_3

CC=g++
LINK=g++
MAKE=make

SRC= config.cpp 	\
     main.cpp           \
     names.cpp          \
     game/anim_row.cpp  \
     game/highscores.cpp \
     game/set.cpp       \
     game/memo.cpp      \
     game/memo_bonus.cpp \
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
     interf/MenuController.cpp  \
     interf/SurvivalController.cpp   \
     interf/TitleController.cpp \
     util/logfile.cpp   \
     util/direct.cpp    \
     util/textfile.cpp	\
     video/animtextevent.cpp    \
     video/background.cpp       \
     video/Display.cpp  \
     video/font.cpp	\
     video/fountainevent.cpp    \
     video/gem.cpp      \
     video/image.cpp	\
     video/imagemanager.cpp	\
     video/inputbox.cpp         \
     video/movableevent.cpp     \
     video/sdlimage.cpp \
     video/sprite.cpp   \
     video/spriteevent.cpp      \
     video/spriteinstance.cpp	\
     video/SDL_rotozoom.cpp     \
     video/textevent.cpp
ifeq ($(USE_OPENGL),yes)
SRC:=$(SRC) video/oglimage.cpp
endif
ifeq ($(USE_SDLMIXER),yes)
SRC:=$(SRC) sound/music.cpp  sound/sound.cpp  sound/soundsystem.cpp 
endif

# libraries
LIBS:=$(shell sdl-config --libs)
LIBS:=$(LIBS) -lSDL_image
#-L/usr/X11R6/lib -lXxf86dga -lXxf86vm -lXv

ifeq ($(USE_EFENCE),yes)
  LIBS:=$(LIBS) -lefence
endif
ifeq ($(USE_OPENGL),yes)
  LIBS:=$(LIBS) -lGL
endif
ifeq ($(USE_SDLMIXER),yes)
  LIBS:=$(LIBS) -lSDL_mixer
endif

# compiler flags
CCFLAGS:=-Wall $(shell sdl-config --cflags)
ifeq ($(USE_OPENGL),no)
  CCFLAGS:=$(CCFLAGS) -DNO_OPENGL
endif
ifeq ($(USE_SDLMIXER),no)
  CCFLAGS:=$(CCFLAGS) -DNO_MUSIC -DNO_SOUND
else
ifeq ($(USE_MUSIC),no)
  CCFLAGS:=$(CCFLAGS) -DNO_MUSIC
endif
ifeq ($(USE_SOUND),no)
  CCFLAGS:=$(CCFLAGS) -DNO_SOUND -DNO_MUSIC
endif
endif
ifeq ($(DEBUG_MODE),yes)
  CCFLAGS:=$(CCFLAGS) -ggdb -DDEBUG -DDEBUG_SANITY_CHECK
endif
ifeq ($(DEBUG_MODE),no)
  CCFLAGS:=$(CCFLAGS) -O2 -DNDEBUG
endif

DCFLAGS=-MM

OBJ:=$(SRC:%.cpp=%.o)
DEP:=$(OBJ:%.o=dep/%.d)

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

# the following targets are for development only (they build tarballs)
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
