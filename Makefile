IDIR=./include
CC=gcc
CXXFLAGS=-I$(IDIR) -O0 -g 
LIBS=-lm -lncurses

SRC=src
BIN=bin
ODIR=src/obj

_DEPS= ui.h file_manager.h gui.h bot_bar.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =  file_manager.o main.o gui.o bot_bar.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
				$(CC) -c -o $@ $< $(CXXFLAGS) 

$(BIN)/main.out : $(OBJ)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm $(BIN)/*.out
