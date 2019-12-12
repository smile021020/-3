SHELL = /bin/bash
CC = gcc
CFLAGS = -g -pthread
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}

%:	%.c
			${CC} ${CFLAGS} $@.c -o $@ -lrt -lpthread

clean:
			rm ${EXE}
				
		
