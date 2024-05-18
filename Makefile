CC=gcc
CFLAGS=-Wall -pedantic -Wextra -Wconversion
BIN=./run

main: main.c
	$(CC) $(CFLAGS) -o $(BIN) main.c

clean:
	rm -rf $(BIN)
