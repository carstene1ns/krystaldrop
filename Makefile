all:
	$(MAKE) -C Sources
	cp Sources/drop .

%: force
	$(MAKE) -C Sources $@
force: ;
