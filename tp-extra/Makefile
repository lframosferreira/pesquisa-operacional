CC = gcc
CFLAGS = -Wall

SIMPLEX = tpextra_2019022553.c

all: $(SIMPLEX)
	$(CC) $(CFLAGS) $(SIMPLEX) -o simplex -lm

test:
	./simplex tests/ENTRADA_example_04

clean:
	rm -f simplex
