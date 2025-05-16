CC = gcc
CFLAGS = -std=c11 -Wall -Iinclude
LIBS = -lSDL2 -ldl -lGL -lm

SRC = main.c
BIN = ./build/sdl-hello

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)

clean:
	rm -f $(BIN)

run:
	$(BIN)

cleanrun:
	rm -f $(BIN) && $(BIN) 

