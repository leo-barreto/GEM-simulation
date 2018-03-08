OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CXX = `root-config --cxx`
CFLAGS = -Wall -Wextra -Wno-long-long \
        `root-config --cflags` \
        -O3 -fno-common \
        -I$(INCDIR) -I$(HEEDDIR)

# Debug flags
# CFLAGS += -g

LDFLAGS = -L$(LIBDIR) -lGarfield
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm
# LDFLAGS += -g

penn: penn.cc
        $(CXX) $(CFLAGS) -c penn.cc
        $(CXX) $(CFLAGS) -o penn penn.o $(LDFLAGS)
        rm penn.o
