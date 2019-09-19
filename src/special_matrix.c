#include "special_matrix.h"

void toeplitz(struct matrix* M){
	int i, j;
	for(i = 0; i < M->row; i++)
	{
		for(j = 0; j < M->col; j++)
		{	
			M->m[i*(M->row)+j] = i > j ? 1 + M->row-  abs(i-j)  : 1 + abs(i-j);
		}
	}
}

void tridiagonal(struct matrix* M){
	int i, j;
	for(i = 0; i < M->row; i++){
		for(j = 0; j < M->col; j++){
			if(abs(i-j) <= 1){
				M->m[i*(M->row)+j] = 1+rand()%6;
			}
			else{
				M->m[i*(M->row)+j] = 0;
			}
		}
	}
}


void triangular(struct matrix* M){
	int i, j;
	if(M->row == M->col)
	{
		for (i = 0; i < M->row; i++) { 
		    for (j = 0; j < M->col; j++) { 
		        if (i > j){ 
		            M->m[i*(M->row)+j] = 0;
		        } 
		        else
		        M->m[i*(M->row)+j] = rand() % 6 + 1; 
		    } 
		} 
	}
	else
		printf("Couldn't be possible plot this matrix, because row != col!");
} 

void vander(struct matrix* M){
	int i, j, k;
	int a[(M->row+1)*(M->row)];
	for(i = 0;i < M->row*M->row; i++){
		a[i] =0;
	}
	
	for(i= 0; i < M->row; i++){
		a[i*M->row] = i+1 ;
	}
	
	for(i =0; i < M->row; i++){
		for(j = 0; j < M->col; j++){
			M->m[i*(M->row)+j] = pow(a[i*M->row], j);
		}
	}
}

void print_special_matrix(struct matrix* M)
{
	printf("\nSpecial Matrix:\n");
	printf("\nToeplitz Matrix\n\n");
	toeplitz(M);
	show_matrix(M);
	printf("Tridiagonal Matrix\n\n");
	tridiagonal(M);
	show_matrix(M);
	printf("Triangular Matrix\n\n");
	triangular(M);
	show_matrix(M);
	printf("Vandermonde Matrix\n\n");
	vander(M);
	show_matrix(M);
	
}
