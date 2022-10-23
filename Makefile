# https://www.gnu.org/software/make/manual/html_node/Simple-Makefile.html#Simple-Makefile
SRC = src/
OBJ = obj/
H = handlers/
GIT = git/
FS = filesystem/

CC = clang
CFLAGS = -Wall -g $(shell pkg-config --cflags libgit2)
LDFLAGS = -lcurl $(shell pkg-config --libs libgit2)
OBJFILES = $(OBJ)main.o $(OBJ)fs.file.o $(OBJ)fs.directory.o $(OBJ)fs.print.o $(OBJ)printutility.o $(OBJ)requests.o $(OBJ)commands.o $(OBJ)input.o $(OBJ)shell.utils.o $(OBJ)git.commands.o $(OBJ)git.remote.o $(OBJ)git.branch.o $(OBJ)git.print.o $(OBJ)git.local.o $(OBJ)git.utils.o $(OBJ)git.credentials.o

TARGET = bin/wizsh

all: $(TARGET)

# LINK
$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# BUILD
$(OBJ)main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)fs.file.o: $(SRC)$(H)$(FS)fs.file.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(FS)fs.file.c -o $(OBJ)fs.file.o

$(OBJ)fs.directory.o: $(SRC)$(H)$(FS)fs.directory.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(FS)fs.directory.c -o $(OBJ)fs.directory.o

$(OBJ)fs.print.o: $(SRC)$(H)$(FS)fs.print.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(FS)fs.print.c -o $(OBJ)fs.print.o

$(OBJ)printutility.o: $(SRC)$(H)printutility.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)printutility.c -o $(OBJ)printutility.o

$(OBJ)requests.o: $(SRC)$(H)requests.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)requests.c -o $(OBJ)requests.o

$(OBJ)shell.utils.o: $(SRC)$(H)shell.utils.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)shell.utils.c -o $(OBJ)shell.utils.o

$(OBJ)git.o: $(SRC)$(H)$(GIT)git.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.c -o $(OBJ)git.o

$(OBJ)git.commands.o: $(SRC)$(H)$(GIT)git.commands.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.commands.c -o $(OBJ)git.commands.o

$(OBJ)git.branch.o: $(SRC)$(H)$(GIT)git.branch.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.branch.c -o $(OBJ)git.branch.o

$(OBJ)commands.o: $(SRC)commands.c
	$(CC) $(CFLAGS) -c $(SRC)commands.c -o $(OBJ)commands.o

$(OBJ)git.credentials.o: $(SRC)$(H)$(GIT)git.credentials.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.credentials.c -o $(OBJ)git.credentials.o

$(OBJ)input.o: $(SRC)input.c
	$(CC) $(CFLAGS) -c $(SRC)input.c -o $(OBJ)input.o

$(OBJ)git.remote.o: $(SRC)$(H)$(GIT)git.remote.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.remote.c -o $(OBJ)git.remote.o

$(OBJ)git.print.o: $(SRC)$(H)$(GIT)git.print.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.print.c -o $(OBJ)git.print.o

$(OBJ)git.local.o: $(SRC)$(H)$(GIT)git.local.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.local.c -o $(OBJ)git.local.o

$(OBJ)git.utils.o: $(SRC)$(H)$(GIT)git.utils.c
	$(CC) $(CFLAGS) -c $(SRC)$(H)$(GIT)git.utils.c -o $(OBJ)git.utils.o

# CLEAN
clean:
	rm -f $(OBJFILES) $(TARGET) *~
