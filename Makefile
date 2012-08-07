#
#	Makefile
#
DATE="`date +%y%m%d%H%M%S`"

#commandline
CMD = -O3 -g
ARGS = 
SPACE = " "

#compiler, linker, archiver
CPP = g++

#compiler flags
LIBS		= -linotifytools -lpthread
CPPINCLUDES 	= -I./include
COMMON_CPPFLAGS = $(CPPINCLUDES)
CPPFLAGS 	= $(COMMON_CPPFLAGS) -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -g3 -fno-exceptions -fno-strict-aliasing -g
LDFLAGS 	= -L. 


#
# section below includes all the necessay rules for building riss
#

#build variables
SRCS = $(wildcard src/*.cc src/*/*.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(SRCS:.cc=.d)
# for building the component libraries
DEPS += $(LIBSRCS:.cc=.d)

all: odb

#build solver
odb: $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) $(LDFLAGS) $(CMD) $(CPPFLAGS) $(ARGS)

#build object file and dependencies files
.cc.o:
	$(CPP) $(CMD) -MD $(CPPFLAGS) $(ARGS) -c -o $@ $<

# clean up backups and old files
clean:
	rm -f *~ */*~ */*/*~
	rm -f $(OBJS) $(DEPS) $(LIBOBJS) $(LIBFILES) $(LIBRISSOBJ)
	rm -f log.txt
	rm -f odb	
# include headerfile dependencies for sources
-include $(DEPS)
