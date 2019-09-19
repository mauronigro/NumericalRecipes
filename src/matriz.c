#include "matriz.h"

static tipoMatriz* init(int row, int col)
{
    tipoMatriz* m = malloc(col*row*sizeof(tipoMatriz));
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
                printf("%.2lf \t", M->m[i*(M->row)+j]);
		}
		printf("\n");
	}
	printf("\n");
}

void destroy_matrix(struct matrix* M)
{
    free(M->m);
	free(M);
}
