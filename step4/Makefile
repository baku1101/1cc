CC = gcc
CFLAGS = -Wall -g -Wextra
OBJ = error.o main.o tokenizer.o parser.o gen.o


%.o: %.c 1cc.h Makefile
	$(CC) $(CFLAGS) -c $*.c

main: $(OBJ) Makefile
	$(CC) $(CFLAGS) -o 1cc $(OBJ)

test: main
	./test.sh

clean:
	rm -rf 1cc *o *~ tmp*
