# Makefile configuration section
# Boolean values must be either 'yes' or 'no'

# OpenGL support ? Speed up a lot the game, and allow some special effect
USE_OPENGL=no

# Link with the SDL_mixer library ? If set to no, then no music
# nor sounds will be played.
USE_SDLMIXER=no

# If USE_SDLMIXER=yes, compile with WAV sound support ?
# (USE_SOUND=no implies USE_MUSIC=no)
USE_SOUND=yes

# If USE_SDLMIXER=yes and USE_SOUND=yes, compile with OGG music support ?
USE_MUSIC=yes

# Profiling switch
# If DISPLAY_FPS=yes, display the approximate frame per second rate ?
DISPLAY_FPS=no

# Debugging switches
# Link with efence ?
USE_EFENCE=no

# Compile with GDB debugging extensions, or with optimizations
DEBUG_MODE=yes

# zip compression support within ACC support
ACC_ZLIB_SUPPORT=yes

# end of the Makefile configuration section
# #########################################

export
VERSION=0_7

CC=g++ -Werror
LINK=g++
MAKE=make

SRC= KrystalDrop/main.cpp                              \
     KrystalDrop/Names.cpp                             \
     KrystalDrop/Controller/BackgroundController.cpp   \
     KrystalDrop/Controller/ControlsController.cpp     \
     KrystalDrop/Controller/DuelController.cpp         \
     KrystalDrop/Controller/SurvivalController.cpp     \
     KrystalDrop/Controller/CharSelectController.cpp   \
     KrystalDrop/Controller/CharSelect2Controller.cpp  \
     KrystalDrop/Controller/KDApplication.cpp          \
     KrystalDrop/Controller/TitleController.cpp        \
     KrystalDrop/Controller/HighScoresController.cpp   \
     KrystalDrop/Controller/MenuController.cpp         \
     KrystalDrop/Game/AnimRow.cpp                      \
     KrystalDrop/Game/Character.cpp                    \
     KrystalDrop/Game/ControlsConfig.cpp               \
     KrystalDrop/Game/Hand.cpp                         \
     KrystalDrop/Game/Highscores.cpp                   \
     KrystalDrop/Game/Memo.cpp                         \
     KrystalDrop/Game/Parameter.cpp                    \
     KrystalDrop/Game/Row.cpp                          \
     KrystalDrop/Game/Set.cpp                          \
     KrystalDrop/Game/Table.cpp                        \
     KrystalDrop/Game/XMLCharacterParser.cpp           \
     KrystalDrop/Tools/ACCArchiveReader.cpp            \
     KrystalDrop/Tools/direct.cpp                      \
     KrystalDrop/Video/Background.cpp                  \
     KrystalDrop/Video/Gem.cpp                         \
     KrystalDrop/Video/InputBox.cpp                    \
     KrystalDrop/Video/Event/AnimTextEvent.cpp         \
     KDpp/Controller/Application.cpp                   \
     KDpp/Controller/Controller.cpp                    \
     KDpp/Controller/WidgetController.cpp              \
     KDpp/Controller/Event.cpp                         \
     KDpp/Controller/EventManager.cpp                  \
     KDpp/Controller/UserInterface/Button.cpp          \
     KDpp/Controller/UserInterface/Frame.cpp           \
     KDpp/Controller/UserInterface/Mouse.cpp           \
     KDpp/Controller/UserInterface/Widget.cpp          \
     KDpp/Controller/UserInterface/CheckBox.cpp        \
     KDpp/Controller/UserInterface/Keyboard.cpp        \
     KDpp/Controller/UserInterface/StaticText.cpp      \
     KDpp/Controller/UserInterface/EditField.cpp       \
     KDpp/Controller/UserInterface/KeyboardCursor.cpp  \
     KDpp/Controller/UserInterface/WidgetContainer.cpp \
     KDpp/Math/Vector.cpp                              \
     KDpp/Resources/ArchiveManager.cpp                 \
     KDpp/Resources/GlobalResourceSet.cpp              \
     KDpp/Resources/Resource.cpp                       \
     KDpp/Resources/ResourceManager.cpp                \
     KDpp/Resources/ResourceSet.cpp                    \
     KDpp/Tools/FilePath.cpp                           \
     KDpp/Tools/Logfile.cpp                            \
     KDpp/Tools/Textfile.cpp                           \
     KDpp/Tools/XMLConfig.cpp                          \
     KDpp/Video/Color.cpp                              \
     KDpp/Video/DisplayableInstance.cpp                \
     KDpp/Video/DisplayableResource.cpp                \
     KDpp/Video/Display.cpp                            \
     KDpp/Video/Font.cpp                               \
     KDpp/Video/Image.cpp                              \
     KDpp/Video/ImageInstance.cpp                      \
     KDpp/Video/SdlImage.cpp                           \
     KDpp/Video/Sprite.cpp                             \
     KDpp/Video/SpriteInstance.cpp                     \
     KDpp/Video/SDL_rotozoom.cpp                       \
     KDpp/Video/Events/DisplayableEvent.cpp            \
     KDpp/Video/Events/FountainEvent.cpp               \
     KDpp/Video/Events/MovableEvent.cpp                \
     KDpp/Video/Events/SpriteEvent.cpp                 \
     KDpp/Video/Events/TextEvent.cpp


# USE_XML is not used anymore: it has to be 'yes'
SRC:=$(SRC) KDpp/Resources/XMLResourceParser.cpp KDpp/Video/XMLSpriteParser.cpp KDpp/Tools/XMLParser.cpp


ifeq ($(USE_OPENGL),yes)
  SRC:=$(SRC) KDpp/Video/OglImage.cpp
endif

ifeq ($(USE_SDLMIXER),yes)
  SRC:=$(SRC) KDpp/Sound/Music.cpp  KDpp/Sound/Sound.cpp  KDpp/Sound/SoundSystem.cpp
endif

# libraries
LIBS:=$(shell sdl-config --libs)
LIBS:=$(LIBS) -lSDL_image -lSDL_ttf

ifeq ($(USE_EFENCE),yes)
  LIBS:=$(LIBS) -lefence
endif
ifeq ($(USE_OPENGL),yes)
  LIBS:=$(LIBS) -lGL
endif
ifeq ($(USE_SDLMIXER),yes)
  LIBS:=$(LIBS) -lSDL_mixer
endif

LIBS:=$(LIBS) $(shell xml2-config --libs) 

# compiler flags
CCFLAGS:=-Wall $(shell sdl-config --cflags) $(shell xml2-config --cflags)

ifeq ($(USE_OPENGL),no)
  CCFLAGS:=$(CCFLAGS) -DNO_OPENGL
endif
ifeq ($(USE_SDLMIXER),no)
  CCFLAGS:=$(CCFLAGS) -DNO_MUSIC -DNO_SOUND -DNO_MIXER
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
ifeq ($(DISPLAY_FPS),yes)
  CCFLAGS:=$(CCFLAGS) -DDISPLAY_FPS
endif

ifeq ($(ACC_ZLIB_SUPPORT),yes)
  CCFLAGS:=$(CCFLAGS) -DACC_ZLIB_SUPPORT
# We also need to link against zlib but libxml2 already includes it
endif


DCFLAGS=-MM

OBJ:=$(SRC:%.cpp=%.o)
DEP:=$(OBJ:%.o=dep/%.d)

all: kdrop

FORCE:

dep: $(DEP)

$(DEP): %.d: FORCE
	@$(MAKE) -s --no-print-directory -f Makefile.dep $@

kdrop: $(OBJ)
	$(LINK) $(LCFLAGS) $(LIBS) -o $@ $(OBJ)

$(OBJ): %.o: dep/%.d
$(OBJ): %.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	find -name "*.o" -exec rm {} ";"
	rm -fR dep
	rm -f log.txt

doc: FORCE
	doxygen krystal.dox

distclean: clean
	rm -fR doc/html
