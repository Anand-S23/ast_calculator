CC = gcc
CFLAGS = -Wall -Wextra -Wno-address -ggdb

default: ast.c
	$(CC) $(CFLAGS) -o ast ast.c

clean:
	rm -f ast
