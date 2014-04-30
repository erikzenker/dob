# Compiler, tools
CC = g++
CC = clang++
DOXYGEN = doxygen

# Build variables
EXECUTABLE=dob
SRCS = $(wildcard src/*.cc)
OBJS = $(SRCS:src/%.cc=bin/%.o)
INCS = -I ./include -I./include/Inotify -I./include/FileStateDatabase

# Compiler flags
CCFLAGS = -c -Wall -std=c++11 
LDFLAGS = 

# Used libs
BOOSTLIBS   = -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system
NEONLIBS    = -lneon
SQLITE3LIBS = -lsqlite3
PTHREADLIBS = -lpthread
SIGCLIBS    = -lsigc-2.0
ALL_LIBS    = $(BOOSTLIBS) $(NEONLIBS) $(SQLITE3LIBS)

# Compilation rules
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) Makefile
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJS) $(ALL_LIBS) -o bin/$@

bin/%.o: src/%.cc $(wildcard include/*.h)
	 $(CC) $< $(CCFLAGS) $(INCS) -o $@

clean:
	rm -f bin/*

new:
	make clean
	make

