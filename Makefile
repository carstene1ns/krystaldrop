all:
	$(MAKE) -C Sources

%: force
	$(MAKE) -C Sources $@
force: ;
