OBJS = src/main.c src/matriz.c src/linear_equation.c src/special_matrix.c src/matrix_operations.c
OBJ_NAME = main
LIBS = -lm
all:
	gcc -o $(OBJ_NAME) $(OBJS) $(LIBS)