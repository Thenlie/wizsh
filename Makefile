# https://www.gnu.org/software/make/manual/html_node/Simple-Makefile.html#Simple-Makefile
SRC = src/
OBJ = obj/

CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lcurl
OBJFILES = $(OBJ)main.o $(OBJ)filesystem.o $(OBJ)printutility.o $(OBJ)requests.o
TARGET = bin/wizsh

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

$(OBJ)main.o: $(SRC)main.c
	$(CC) -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)filesystem.o: $(SRC)filesystem.c
	$(CC) -c $(SRC)filesystem.c -o $(OBJ)filesystem.o

$(OBJ)printutility.o: $(SRC)printutility.c
	$(CC) -c $(SRC)printutility.c -o $(OBJ)printutility.o

$(OBJ)requests.o: $(SRC)requests.c
	$(CC) -c $(SRC)requests.c -o $(OBJ)requests.o

clean:
	rm -f $(OBJFILES) $(TARGET) *~