OBJ = main.o
all: projekt
projekt: $(OBJ)
	gcc $(OBJ) -o projekt
