CC = gcc
CFLAGS = -Wall

SIMPLEX = tpextra_2019022553.c

all: $(SIMPLEX)
	$(CC) $(CFLAGS) $(SIMPLEX) -o simplex -lm

test:
	./simplex simplex_testes/Testes/01

clean:
	rm -f simplex
