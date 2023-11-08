CC = gcc
CFLAGS = -Wall

SIMPLEX = tpextra_2019022553.c

all: $(SIMPLEX)
	$(CC) $(CFLAGS) $(SIMPLEX) -o simplex

test:
	./simplex tests/ENTRADA_example_03

clean:
	rm -f simplex
