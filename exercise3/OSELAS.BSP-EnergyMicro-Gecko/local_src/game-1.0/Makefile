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
CC = arm-cortexm3-uclinuxeabi-gcc

all: game.o pong_graphics.o
	$(CC) -o game game.o pong_graphics.o -I.

clean:
	-rm -f game game.o pong_graphics.o

install:


.PHONY: all install clean

# end of development makefile
