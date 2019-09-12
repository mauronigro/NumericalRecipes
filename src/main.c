#include "main.h"
#include <mpi.h>
int main(int argc, char* argv[])
{
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int p = atoi(argv[3]);
	int q = atoi(argv[4]);
	int rank, 
	    task, 
	    numworker, 
	    source, 
	    dest, 
	    rows_for_send;
	struct matrix* M = init_matrix(m, n);
	struct matrix* N = init_matrix(p, q);
	toeplitz(M);
	tridiagonal(N);
	show_matrix(M);
	show_matrix(N);
	MPI_Init(&argc, &argv);
	M = multiply_matrix(M , N);

	MPI_Finalize();
	show_matrix(M);
	destroy_matrix(M);
	return 0;
}

