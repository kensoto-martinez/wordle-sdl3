SRC=src/main.c src/rendering.c src/config.c
EXE=build/wordle

all:
	gcc $(SRC) -o $(EXE) -Iinclude -Llib -lSDL3 -lSDL3_ttf -lSDL3_image
	$(EXE)

clean:
	rm -f $(EXE)