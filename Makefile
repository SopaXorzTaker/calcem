PRODUCT := nxu8test
SRCDIR := src
DEPDIR := dep
BINDIR := bin
OBJDIR := obj

CC := gcc
CCEXT := c
#CCFLAGS := -Wall -std=c11
CCFLAGS := -Wall -std=c11 -g

FILES := $(shell find $(SRCDIR) -name *.$(CCEXT))

# RELATIVEPATH only affects GCC output

$(PRODUCT): $(patsubst %,$(OBJDIR)/%.o,$(subst /,__,$(patsubst $(SRCDIR)/%,%,$(FILES))))
	@$(CC) -o $(BINDIR)/$(PRODUCT) $(patsubst %,$(OBJDIR)/%.o,$(subst /,__,$(patsubst $(SRCDIR)/%,%,$(FILES)))) $(CCFLAGS)

-include $(patsubst %,$(DEPDIR)/%.dep,$(subst /,__,$(patsubst $(SRCDIR)/%,%,$(FILES))))

$(OBJDIR)/%.$(CCEXT).o:
	@$(eval CPATH := $(patsubst %,$(SRCDIR)/%,$(subst __,/,$*.$(CCEXT))))
	@$(eval DPATH := $(patsubst %,$(DEPDIR)/%,$*.$(CCEXT)).dep)
	@$(eval RELBACK := `echo $(RELATIVEPATH) | sed -e 's/[^/][^/]*/\.\./g'`)
	@cd $(RELATIVEPATH). && $(CC) $(CCFLAGS) -c $(RELBACK)$(CPATH) -o $(RELBACK)$(OBJDIR)/$*.$(CCEXT).o
	@$(CC) $(CCFLAGS) -MM $(CPATH) > $(DPATH)
	@mv -f $(DPATH) $(DPATH).tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.$(CCEXT).o:|' < $(DPATH).tmp > $(DPATH)
	@sed -e 's/.*://' -e 's/\\$$//' < $(DPATH).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(DPATH)
	@rm -f $(DPATH).tmp

clean:
	@rm -f $(BINDIR)/$(PRODUCT) $(OBJDIR)/*.o $(DEPDIR)/*.dep
