# https://www.gnu.org/software/make/manual/html_node/Simple-Makefile.html#Simple-Makefile
SRC = src/
OBJ = obj/
H = handlers/
GIT = git/

CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lcurl
OBJFILES = $(OBJ)main.o $(OBJ)filesystem.o $(OBJ)printutility.o $(OBJ)requests.o $(OBJ)commands.o $(OBJ)input.o $(OBJ)git.o
TARGET = bin/wizsh

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

$(OBJ)main.o: $(SRC)main.c
	$(CC) -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)filesystem.o: $(SRC)$(H)filesystem.c
	$(CC) -c $(SRC)$(H)filesystem.c -o $(OBJ)filesystem.o

$(OBJ)printutility.o: $(SRC)$(H)printutility.c
	$(CC) -c $(SRC)$(H)printutility.c -o $(OBJ)printutility.o

$(OBJ)requests.o: $(SRC)$(H)requests.c
	$(CC) -c $(SRC)$(H)requests.c -o $(OBJ)requests.o

$(OBJ)git.o: $(SRC)$(GIT)git.c
	$(CC) -c $(SRC)$(GIT)git.c -o $(OBJ)git.o

$(OBJ)commands.o: $(SRC)commands.c
	$(CC) -c $(SRC)commands.c -o $(OBJ)commands.o

$(OBJ)input.o: $(SRC)input.c
	$(CC) -c $(SRC)input.c -o $(OBJ)input.o

clean:
	rm -f $(OBJFILES) $(TARGET) *~
