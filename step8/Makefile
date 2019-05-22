CC = gcc
CFLAGS = -Wall -g -Wextra -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

$(OBJS): 1cc.h

1cc: $(OBJS)
	$(CC) -o 1cc $(OBJS) $(LDFLAGS)

test: 1cc
	./1cc -test
	./test.sh

clean:
	rm -rf 1cc *o *~ tmp*
