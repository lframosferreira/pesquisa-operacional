CC = gcc
LIBS = -lm
BIN = bin
OBJ = obj
CFLAGS = -Wall

SIMPLEX = $(BIN)/simplex


all: $(SIMPLEX)

$(SIMPLEX): simplex.c
	test -d $(BIN) || mkdir $(BIN)
	$(CC) $(CFLAGS) simplex.c -o $(SIMPLEX) $(LIBS)

clean:
	rm -f $(BIN)/*
