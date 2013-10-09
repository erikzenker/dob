# compiler, tools
CC = clang
CC = g++
DOXYGEN = doxygen

# build variables
EXECUTABLE=dob
SRCS = $(wildcard src/*.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(SRCS:.cc=.d)
INCS = -I./include $(shell pkg-config --cflags sigc++-2.0)

# compiler flags
CFLAGS = -c -Wall -std=c++0x
LDFLAGS = 

# Used libs
BOOSTLIBS = -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system
NEONLIBS    = -lneon
SQLITE3LIBS = -lsqlite3
PTHREADLIBS = -lpthread
SIGCLIBS    = -lsigc-2.0
LIBS	    = $(PTHREADLIBS) $(BOOSTLIBS) $(NEONLIBS) $(SQLITE3LIBS) $(SIGCLIBS)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) 
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCS) $< -o $@

# clean up backups and old files
clean:
	rm -f *~ */*~ */*~
	rm -f $(OBJS) $(DEPS)
	rm -f $(EXECUTABLE)

new:
	make clean
	make all

# generate documentation
doc:
	$(DOXYGEN) Doxygen.conf

