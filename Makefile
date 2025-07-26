SRC=src/main.c src/config.c src/game.c
EXE=build/wordle

all:
	gcc $(SRC) -o $(EXE) -Iinclude -Llib -lSDL3 -lSDL3_ttf -lSDL3_image
	$(EXE)