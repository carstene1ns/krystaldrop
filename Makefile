VERSION=0.7

DESTDIR=
CONFDIR=$(DESTDIR)/etc
EXEDIR=$(DESTDIR)/usr/games
GAMESDIR=$(DESTDIR)/usr/share/games
BINDIR=$(DESTDIR)/usr/share/games/krystaldrop
ARTDIR=$(DESTDIR)/usr/share/games/krystaldrop/art
ART_SUBDIRS= UI UI/button UI/checkbox UI/editfield UI/window \
             characters chibi cup gems line menu sound star survival title
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
	install -m644 art/*.ogg art/*.txt art/*.sco art/*.png art/*.TTF $(ARTDIR)
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
	install -m644 kdrop.xml $(CONFDIR)

%: force
	$(MAKE) -C Sources $@
force: ;

pack_src:
	@make clean
	@mkdir -p $(PACKDIR)
	@rm -f $(PACKDIR)/src_$(VERSION).tgz
	tar -C .. --exclude "*/CVS*" --exclude "*/art/*" -cvzf $(PACKDIR)/src_$(VERSION).tgz krystaldrop/BUGLIST krystaldrop/COPYING krystaldrop/Makefile krystaldrop/Makefile.dep krystaldrop/README krystaldrop/krystal.dox krystaldrop/Sources

pack_art:
	@mkdir -p $(PACKDIR)
	@rm -f $(PACKDIR)/art_$(VERSION).tgz
	tar -cvzf $(PACKDIR)/art_$(VERSION).tgz art/

pack_deb_no_gpg:
	@make clean
	@mkdir -p $(PACKDIR)
	@ln -sf Installers/debian
	dpkg-buildpackage -rfakeroot -b -ikrystaldrop-$(VERSION)/$(PACKDIR)/*
	mv ../krystaldrop_$(VERSION)-1_i386.deb $(PACKDIR)

.PHONY: all clean install pack_src pack_art pack_deb_no_gpg force
