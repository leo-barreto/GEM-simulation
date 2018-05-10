OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CXX = `root-config --cxx`
CFLAGS = -Wall -Wextra -Wno-long-long \
        `root-config --cflags` \
        -O3 -fno-common  \
        -I$(INCDIR) -I$(HEEDDIR)

# Debug flags
#CFLAGS += -g

LDFLAGS = -L$(LIBDIR) -lGarfield
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm
#LDFLAGS += -g


potential: potential.cc
	$(CXX) $(CFLAGS) -c potential.cc
	$(CXX) $(CFLAGS) -o potential potential.o $(LDFLAGS)
	rm potential.o
