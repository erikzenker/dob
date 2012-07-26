EXEC            := statusicon

CXX		:= g++
CXXFLAGS        := -O2 -g
CXXFLAGS	+= -Wall -ansi
CXXFLAGS	+= $(shell pkg-config --cflags gtkmm-2.4)
LIBS		:= $(shell pkg-config --libs gtkmm-2.4)

SRC		:= examplewindow.cc main.cc
OBJ		:= $(addsuffix .o, $(basename $(SRC)))

all:		$(OBJ)
		$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(EXEC)

.cc:
		$(CXX) $(CXXFLAGS) -c $@

clean :		
		@rm -fv $(EXEC)
		@rm -fv $(OBJ)
