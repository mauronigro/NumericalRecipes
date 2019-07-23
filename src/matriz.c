#include "matriz.h"

tipoMatriz** init(int row, int col)
{
	tipoMatriz** m = malloc(row*sizeof(tipoMatriz));
	for(int i = 0; i < row; i++)
	{
		m[i] = malloc(col*sizeof(tipoMatriz*));
	}
	return m;
}

struct matrix* init_matrix(int row, int col)
{
	struct matrix* M = malloc(sizeof(struct matrix));
	M->row = row;
	M->col = col;
	M->m = init(row, col);
	return M;
}

void show_matrix(struct matrix* M)
{
	int i, j;
	for(i = 0; i < M->row; i++)
	{
		for(j = 0; j < M->col; j++)
		{
			printf("%.2lf \t", M->m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void destroy_matrix(struct matrix* M)
{
	for(int i = 0; i < M->row; i++)
		free(M->m[i]);
	free(M->m);
	free(M);
}
