#
# Make all of the projects in the subdirectories
#

SUBDIRS = SetupGPIO/ Method1/ Method2/ Method3/ Method4/ 
all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	@for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean $(SUBDIRS)

