# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
	`root-config --cflags` \
	-O3 -fno-common -c \

# Debug flags
# CFLAGS += -g

LDFLAGS = `root-config --glibs` -lGeom -lgfortran -lm

writeinfo: writeinfo.cc
	$(CXX) $(CFLAGS) writeinfo.cc
	$(CXX) -o writeinfo writeinfo.o $(LDFLAGS)
	rm writeinfo.o
