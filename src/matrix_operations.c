#include "matrix_operations.h"
struct matrix* multiply_matrix(struct matrix* A, struct matrix* B){
	int i, j,k;
	tipoMatriz sum;
	struct matrix* RESULT = init_matrix(A->row, B->col);
	
	for (i = 0; i < RESULT->row; i++) 
	{
            for (j = 0; j < RESULT->col; j++) 
			{
            	sum = 0;
            	for (k = 0; k < A->col; k++) // or B->row , because A->col == B->row
				{
                	sum += A->m[i][k] * B->m[k][j];
                }
            	RESULT->m[i][j] = sum;
			}	
	}
	return RESULT;
}