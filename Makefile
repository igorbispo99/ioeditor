IDIR=./include
CC=g++
CXXFLAGS=-I$(IDIR) -O2 -g -std=c++14
LIBS=-lncurses

SRC=src
BIN=bin
ODIR=src/obj

_DEPS= file_manager.hpp gui.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = file_manager.o main.o gui.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
				$(CC) -c -o $@ $< $(CXXFLAGS) 

$(BIN)/main : $(OBJ)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm $(BIN)/*
