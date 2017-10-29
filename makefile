SUBDIRS := show mesher
UTILITY := IO exception

all: $(UTILITY) $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@


$(UTILITY):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) $(UTILITY)
