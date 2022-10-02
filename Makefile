# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# wizsh: main.c main.h
# 	clang -o wizsh main.c

COMPILER=clang
CFLAGS=-I.
DEPS = main.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wizsh: main.c utils.c
	$(CC) -o wizsh main.c