SRC1 ?= src/main.c
SRC2 ?= src/config.c
SRC3 ?= src/game_functions.c
EXE = build/wordle

all:
	gcc $(SRC1) $(SRC2) $(SRC3) -o $(EXE) -Iinclude -Llib -lSDL3 -lSDL3_ttf -lSDL3_image && $(EXE)

clean:
	rm -f $(EXE)