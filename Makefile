# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# COMPILER = clang
# CFLAGS = -g -Wall
# DEPS = main.h
SRC = src/

# wizsh: ${SRC}*.c
# 	$(CC) -o bin/wizsh ${SRC}main.c

# clean:
# 	$(RM) -r bin/*





CC = clang
CFLAGS = -Wall
LDFLAGS =
OBJFILES = main.o filesystem.o printutility.o
TARGET = wizsh

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

main.o: ${SRC}main.c
	$(CC) -c ${SRC}main.c

filesystem.o: ${SRC}filesystem.c
	$(CC) -c ${SRC}filesystem.c

printutility.o: ${SRC}printutility.c
	$(CC) -c ${SRC}printutility.c

clean:
	rm -f $(OBJFILES) $(TARGET) *~