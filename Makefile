# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
COMPILER = clang
CFLAGS = -g -Wall -I
DEPS = main.h
SRC = src/

wizsh: ${SRC}main.c ${SRC}utils.c
	$(CC) -o bin/wizsh ${SRC}main.c