SOURCE = src/main.c src/matriz.c src/linear_equation.c src/special_matrix.c src/matrix_operations.c
BIN = main
LIBS = -lm
CC = mpicc
all:
	$(CC) -o $(BIN) $(SOURCE) $(LIBS)
