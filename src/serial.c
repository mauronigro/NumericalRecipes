#include "main.h"
#include <mpi.h>
int main(int argc, char* argv[])
{
	int m = atoi(argv[1]),
	    n = atoi(argv[2]),
	    p = atoi(argv[3]),
	    q = atoi(argv[4]);
	MPI_Init(&argc, &argv);
	double t1 = MPI_Wtime();
	struct matrix* A = init_matrix(m,n);
	struct matrix* B = init_matrix(p,q);
	struct matrix* C = init_matrix(m,q);
	toeplitz(A);
	triangular(B);
	C = multiply_matrix(A,B);
	destroy_matrix(A);
	destroy_matrix(B);
	destroy_matrix(C);
	double t2 = MPI_Wtime();
	MPI_Finalize();
	printf("Tempo de execuação da multiplicação matricial(Serial): %.20lf segundos\n ", t2-t1);

}
