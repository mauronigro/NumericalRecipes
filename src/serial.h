#include "common.h"

// From matriz.c
extern struct matrix* init_matrix(int row, int col);
extern void show_matrix(struct matrix* M);
extern void destroy_matrix(struct matrix* M);

// From special_matrix.c
extern void toeplitz(struct matrix* M);
extern void tridiagonal(struct matrix* M);
extern void triangular(struct matrix* M);
extern void vander(struct matrix* M);

// From matrix_operations
extern struct matrix* multiply_matrix(struct matrix* A, struct matrix* B);
