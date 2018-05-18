CC = gcc
REP = bin
CFLAGS = -Wall
EXEC = bin/tsh bin/new bin/list bin/rmall bin/size bin/newdir bin/fin

.PHONY: all clean

all: $(EXEC)

bin/tsh: src/tsh.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/tsh src/tsh.c 
	
bin/new: src/new.c
	$(CC) $(CFLAGS) -o bin/new src/new.c

bin/list: src/list.c
	$(CC) $(CFLAGS) -o bin/list src/list.c

bin/rmall: src/rmall.c
	$(CC) $(CFLAGS) -o bin/rmall src/rmall.c

bin/size: src/size.c
	$(CC) $(CFLAGS) -o bin/size src/size.c

bin/newdir: src/newdir.c
	$(CC) $(CFLAGS) -o bin/newdir src/newdir.c

bin/fin: src/fin.c
	$(CC) $(CFLAGS) -o bin/fin src/fin.c

clean:
	rm -f $(EXEC) $(wildcard src/*.o) 
