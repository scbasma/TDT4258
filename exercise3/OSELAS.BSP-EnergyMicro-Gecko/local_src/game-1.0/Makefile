#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=
CC = gcc

game: game.o pong_graphics.o pong_logic.o;$(CC) -o game game.o pong_graphics.o pong_logic.o -I.

clean:
	-rm -f game game.o pong_graphics.o pong_logic.o

install:


.PHONY: all install clean

# end of development makefile
