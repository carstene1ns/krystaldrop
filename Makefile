#next version: VERSION=0.7.2
VERSION=0.7

DESTDIR=
CONFDIR=$(DESTDIR)/etc
EXEDIR=$(DESTDIR)/usr/games
GAMESDIR=$(DESTDIR)/usr/share/games
BINDIR=$(DESTDIR)/usr/share/games/krystaldrop
ARTDIR=$(DESTDIR)/usr/share/games/krystaldrop/art
ART_SUBDIRS= UI UI/button UI/checkbox UI/editfield UI/window \
             characters chibi cup gems line menu musics fonts sound star survival title
CHARACTERS= chaos darkness fire forest light snow space time water wind
PACKDIR=pack


all:
	$(MAKE) -C Sources
	cp Sources/kdrop .

clean:
	$(MAKE) -C Sources clean
	@rm -f kdrop

install:
	install -d $(GAMESDIR) $(BINDIR) $(ARTDIR) $(CONFDIR) $(EXEDIR) 
# The standard configuration file points art/ relatively to ./
# We want an absolute location for a UNIX install, so we have to change the directory on the fly
	sed 's*<base\_directory>art</base\_directory>*<base\_directory>/usr/share/games/krystaldrop/art</base\_directory>*' kdrop.xml > $(CONFDIR)/kdrop.xml
# The highscore files are stored twice. The copies in the art directory are read-only backups to restore deleted or corrupted high scores files
	install -m644 *.sco $(BINDIR)
	install -m644 *.sco $(ARTDIR)
	install -m644 art/*.txt art/*.png $(ARTDIR)
	for dir in $(ART_SUBDIRS); do \
	 install -d $(ARTDIR)/$$dir; \
	 for file in art/$$dir/*.*; do \
	  install -m644 $$file $(ARTDIR)/../$$file; \
	 done; \
	done
	- for dir in $(CHARACTERS); do \
	 install -d $(ARTDIR)/characters/$$dir; \
	 install -m644 art/characters/$$dir/*.xml art/characters/$$dir/$$dir* $(ARTDIR)/characters/$$dir; \
	 install -d $(ARTDIR)/characters/$$dir/chibi $(ARTDIR)/characters/$$dir/voices; \
	 for file in art/characters/$$dir/chibi/*; do \
	  install -m644 $$file $(ARTDIR)/characters/$$dir/chibi; \
	 done; \
	 for file in art/characters/$$dir/voices/*; do \
	  install -m644 $$file $(ARTDIR)/characters/$$dir/voices; \
	 done; \
	done
	install $(INSTALL_OPT) kdrop $(EXEDIR)

%: force
	$(MAKE) -C Sources $@
force: ;

doc: force
	doxygen doc/krystal.dox

pack_src:
	@make clean
	@mkdir -p $(PACKDIR)
	@rm -f $(PACKDIR)/src_$(VERSION).tgz
	tar -C .. --exclude "*/CVS*" --exclude "*/art/*" -cvzf $(PACKDIR)/src_$(VERSION).tgz krystaldrop/CHANGES krystaldrop/COPYING krystaldrop/Makefile krystaldrop/Makefile.dep krystaldrop/README krystaldrop/doc krystaldrop/Sources

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
