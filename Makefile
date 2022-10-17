# https://www.gnu.org/software/make/manual/html_node/Simple-Makefile.html#Simple-Makefile
SRC = src/
OBJ = obj/
H = handlers/
GIT = git/

CC = clang
CFLAGS = -Wall -g $(shell pkg-config --cflags libgit2)
LDFLAGS = -lcurl $(shell pkg-config --libs libgit2)
OBJFILES = $(OBJ)main.o $(OBJ)filesystem.o $(OBJ)printutility.o $(OBJ)requests.o $(OBJ)commands.o $(OBJ)input.o $(OBJ)git.o $(OBJ)git-commands.o $(OBJ)git.remote.o

TARGET = bin/wizsh

all: $(TARGET)

# LINK
$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# BUILD
$(OBJ)main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)filesystem.o: $(SRC)$(H)filesystem.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)filesystem.c -o $(OBJ)filesystem.o

$(OBJ)printutility.o: $(SRC)$(H)printutility.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)printutility.c -o $(OBJ)printutility.o

$(OBJ)requests.o: $(SRC)$(H)requests.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)requests.c -o $(OBJ)requests.o

$(OBJ)git.o: $(SRC)$(GIT)git.c
	$(CC) $(CFLAGS) -c $(SRC)$(GIT)git.c -o $(OBJ)git.o

$(OBJ)git-commands.o: $(SRC)$(GIT)git-commands.c
	$(CC) $(CFLAGS) -c $(SRC)$(GIT)git-commands.c -o $(OBJ)git-commands.o

$(OBJ)commands.o: $(SRC)commands.c
	$(CC) $(CFLAGS) -c $(SRC)commands.c -o $(OBJ)commands.o

$(OBJ)input.o: $(SRC)input.c
	$(CC) $(CFLAGS) -c $(SRC)input.c -o $(OBJ)input.o

$(OBJ)git.remote.o: $(SRC)$(GIT)git.remote.c
	$(CC) $(CFLAGS) -c $(SRC)$(GIT)git.remote.c -o $(OBJ)git.remote.o

# CLEAN
clean:
	rm -f $(OBJFILES) $(TARGET) *~
