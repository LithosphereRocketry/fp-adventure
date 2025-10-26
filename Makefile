# Semi-automated C/C++ makefile

# This should be all you need to change
TARGETS = add_fuzz sub_fuzz unit_tests

INCDIR = include/
# C/C++ files go here
SRCDIR = src/
# Generated object files go here
OBJDIR = obj/

COPTS = -O2

# if you just type "make", this happens
.DEFAULT_GOAL=default
# these rules don't refer to real files, so they shouldn't get timestamp-checked
.PHONY: all clean test default

default: all test

# make all = make (all of the final targets given)
all: $(TARGETS)

test: unit_tests
	./$<

# includes = everything in INCDIR with a .h file extension
INC = $(wildcard $(INCDIR)*.h)
# C source files = SRCDIR with a .c file extension
CSRC = $(wildcard $(SRCDIR)*.c)
# Combine the list of filenames from CSRC but change them to OBJDIR/whatever.o for compiled objects
OBJ = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(CSRC))
# Target objects = things that need libraries given to them = convert targets to OBJDIR/whatever.o
TGTOBJ = $(patsubst %,$(OBJDIR)%.o,$(TARGETS))
# Dependency objects = things that should be given to targets = whatever objects aren't targets
DEPOBJ = $(filter-out $(TGTOBJ),$(OBJ))

# To build a target:
# - Require its corresponding object and all of the dependency objects to be up to date
# - Throw everything to the system's default C++ compiler
$(TARGETS): %: $(OBJDIR)%.o $(DEPOBJ)
	$(CC) $(COPTS) -o $@ $< $(DEPOBJ)

# To build an object:
# - require its .c file to be up to date, all includes to be up to date, and the OBJDIR folder to exist
# - Throw it and the includes to default C compiler
$(OBJDIR)%.o: $(SRCDIR)%.c $(INC) | $(OBJDIR)
	$(CC) $(COPTS) -I$(INCDIR) -c $< -o $@

# If the object directory doesn't exist, make it
$(OBJDIR):
	mkdir $@

# If everything else fails, warn the user
% ::
	@echo "File $@ missing or not buildable"

# clean deletes all objects and target executables
clean:
	rm -f $(OBJDIR)* $(TARGETS)
