# Krystal Drop root Makefile
# This makefile controls the installation process and various packaging targets
# Look for the Sources/Makefile for options regarding the compilation itself

VERSION=0.7.2

# GAME_PREFIX is the prefix for all the files krystal drop will look for.
GAME_PREFIX:=

# INSTALL_PREFIX/GAME_PREFIX is the destination directory for the `install' target
INSTALL_PREFIX:=

# If you are packaging kdrop, you may want:
# GAME_PREFIX:=
# INSTALL_PREFIX:=some_temporary_directory

# If you are not root or do not want to install the game as root, you may want:
# GAME_PREFIX:=/home/meow/bin/kdrop
# INSTALL_PREFIX:=
# You may also want to change CFGDIR, EXEDIR, BINDIR into something like this:
# CFGDIR:=$(GAME_PREFIX)
# EXEDIR:=$(GAME_PREFIX)
# BINDIR:=$(GAME_PREFIX)
CFGDIR:=$(GAME_PREFIX)/etc
EXEDIR:=$(GAME_PREFIX)/usr/games
BINDIR:=$(GAME_PREFIX)/usr/share/games/krystaldrop

# ############################################################################
# You should not need to change anything below this line
ARTDIR:= $(BINDIR)/art
INSTALL_ARTDIR:= $(INSTALL_PREFIX)/$(ARTDIR)
INSTALL_BINDIR:= $(INSTALL_PREFIX)/$(BINDIR)
INSTALL_CFGDIR:= $(INSTALL_PREFIX)/$(CFGDIR)
INSTALL_EXEDIR:= $(INSTALL_PREFIX)/$(EXEDIR)

ART_SUBDIRS:= UI/button UI/checkbox UI/editfield UI/window characters musics
CHARACTERS:= chaos darkness fire forest light snow space time water wind
PACKDIR:= pack

all:
	$(MAKE) -C Sources CFGDIR="$(CFGDIR)" BINDIR="$(BINDIR)"
	mv Sources/kdrop .

clean:
	$(MAKE) -C Sources clean
	@rm -f kdrop

install:
	install -d $(INSTALL_ARTDIR) $(INSTALL_CFGDIR) $(INSTALL_EXEDIR) 
# The standard configuration file defines the art directory to be `art/'
# We want an absolute location for a UNIX install, so we have to change the directory on the fly
	sed 's*<base\_directory>art</base\_directory>*<base\_directory>$(ARTDIR)</base\_directory>*' kdrop.xml > $(INSTALL_CFGDIR)/kdrop.xml
# The highscore files are stored twice. The copies in the art directory are read-only backups to restore deleted or corrupted high scores files
	install -m644 *.sco $(INSTALL_BINDIR)
	install -m644 *.sco $(INSTALL_ARTDIR)
	install -m644 art/*.txt art/*.png art/*.acc $(INSTALL_ARTDIR)
	for dir in $(ART_SUBDIRS); do \
	 install -d $(INSTALL_ARTDIR)/$$dir; \
	 for file in art/$$dir/*.*; do \
	  install -m644 $$file $(INSTALL_ARTDIR)/../$$file; \
	 done; \
	done
	- for dir in $(CHARACTERS); do \
	 install -d $(INSTALL_ARTDIR)/characters/$$dir; \
	 install -m644 art/characters/$$dir/*.xml art/characters/$$dir/$$dir* $(INSTALL_ARTDIR)/characters/$$dir; \
	 install -d $(INSTALL_ARTDIR)/characters/$$dir/chibi $(INSTALL_ARTDIR)/characters/$$dir/voices; \
	 for file in art/characters/$$dir/chibi/*; do \
	  install -m644 $$file $(INSTALL_ARTDIR)/characters/$$dir/chibi; \
	 done; \
	 for file in art/characters/$$dir/voices/*; do \
	  install -m644 $$file $(INSTALL_ARTDIR)/characters/$$dir/voices 2> /dev/null; \
	 done; \
	done
	@echo "** Do not worry if make reported errors when installing resources, some characters voices are still missing **"	
	install $(INSTALL_OPT) kdrop $(INSTALL_EXEDIR)

%: force
	$(MAKE) -C Sources $@
force: ;

doc: force
	doxygen doc/krystal.dox

pack_src:
	@make clean
	@mkdir -p $(PACKDIR)
	@rm -f $(PACKDIR)/src_$(VERSION).tgz
	tar -C .. --exclude "*/CVS*" --exclude "*/art/*" -cvzf $(PACKDIR)/src_$(VERSION).tgz \
	 krystaldrop/CHANGES krystaldrop/COPYING krystaldrop/Makefile krystaldrop/Makefile.dep \
	 krystaldrop/README krystaldrop/survival.sco krystaldrop/kdrop.xml \
	 krystaldrop/doc krystaldrop/Sources

pack_art:
	@mkdir -p $(PACKDIR)
	@rm -f $(PACKDIR)/art_$(VERSION).tgz
	tar -cvzf $(PACKDIR)/art_$(VERSION).tgz art/

pack_deb_no_gpg:
	@make clean
	@mkdir -p $(PACKDIR)
	@ln -sf Installers/debian
	dpkg-buildpackage -rfakeroot -b -ikrystaldrop-$(VERSION)/$(PACKDIR)/* -us -uc
	mv ../krystaldrop_$(VERSION)-1_i386.deb $(PACKDIR)

.PHONY: all clean install pack_src pack_art pack_deb_no_gpg force
