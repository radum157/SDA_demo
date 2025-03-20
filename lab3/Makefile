CFLAGS := -Wall -Wextra -pedantic -fPIC -O2 -I./include
LDFLAGS :=
CC := gcc

EXE := main
SRC := lib/list.c main.c

build:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAGS)

run: build
	./$(EXE)

clean:
	rm -f $(EXE)

valrun: build
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(EXE)

pack:
	zip -FSr demo.zip $(SRC) include/ main.c Makefile

.PHONY: build run clean valrun pack
