SOURCE = src/main.c src/matriz.c src/linear_equation.c src/special_matrix.c src/matrix_operations.c
BIN = main
LIBS = -lm -O3
CC = mpicc
all:
	$(CC) -o $(BIN) $(SOURCE) $(LIBS)
	mpirun -n 8 ./$(BIN) 1000 1000 1000 1000 
	$(CC) -o serial src/serial.c src/matriz.c src/linear_equation.c src/special_matrix.c src/matrix_operations.c -lm -O3
	mpirun -n 1 ./serial 1000 1000 1000 1000
