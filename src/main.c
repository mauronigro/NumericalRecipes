#include "main.h"
#include <mpi.h>
/*
 *  MPI_Send(
 *          void* data,
 *          int count,
 *          MPI_Datatype datatype,
 *          int destination,
 *          int tag,
 *          MPI_Comm communicator)
 *  MPI_Recv(
 *          void* data,
 *          int count,
 *          MPI_Datatype datatype,
 *          int source,
 *          int tag,
 *          MPI_Comm communicator,
 *          MPI_Status* status)   
 *  int MPI_Gather(
 *          const void *sendbuf, 
 *          int sendcount, 
 *          MPI_Datatype sendtype,
 *          void *recvbuf, int recvcount, MPI_Datatype recvtype,
 *          int root, 
 *          MPI_Comm comm)
 *  int MPI_Scatter(
 *          const void *sendbuf, 
 *          int sendcount, 
 *          MPI_Datatype sendtype,
 *          void *recvbuf, 
 *          int recvcount, 
 *          MPI_Datatype recvtype, 
 *          int root,
 *          MPI_Comm comm)
 *
 * */
int main(int argc, char* argv[])
{
    /* Variáveis de entrada */
    int m = atoi(argv[1]), 
        n = atoi(argv[2]), 
        p = atoi(argv[3]), 
        q = atoi(argv[4]);
    /* Condição para matriz poder ser multiplicada */
    if(n != p)
        return 0;
    /* Variáveis iniciais */
    int task, 
        num_tasks, 
        num_slaves,
        source, 
        dest,
        i,
        j,
        k,
        offset,
        extra,
        rows,
        rows_for_send,
        rows_for_recv;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &task);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Status status;

    tipoMatriz* tmp;
    tipoMatriz* result;
    struct matrix* B = init_matrix(p, q);
    triangular(B);

    num_slaves = num_tasks - 1;
    rows = m / num_slaves;
    extra = m % num_slaves;

    if(task == 0)
    {
	/* Matriz A a ser multiplicada */
        struct matrix* A = init_matrix(m, n);
        struct matrix* C = init_matrix(m, q);
        struct matrix* D = init_matrix(m, q);
        toeplitz(A);
        C = multiply_matrix(A, B);
        show_matrix(A);
        show_matrix(B);
        show_matrix(C);
        offset = 0;
        for(dest = 1; dest <= num_slaves; dest++)
        {

            rows_for_send = (dest <= extra) ? rows + 1 : rows;
            tmp = malloc(n*rows_for_send*sizeof(tipoMatriz));
            // atribui ao vetor temporário (tmp) a ser enviado;
            for(i = 0; i < n*rows_for_send; i++)
                tmp[i] = A->m[offset*m+i];
            MPI_Send(tmp, n*rows_for_send, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&offset , 1 , MPI_INT, dest, 0, MPI_COMM_WORLD);
            offset = offset + rows_for_send;
            free(tmp);
        }
        offset = 0;
        for(dest = 1; dest <= num_slaves; dest++)
        {
            rows_for_recv = (dest <= extra) ? rows + 1 : rows;
            result = malloc(n*rows_for_recv*sizeof(tipoMatriz));
            MPI_Recv(result, n*rows_for_recv, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD, &status);
            for(i = 0; i < rows_for_recv; i++)
            {
                for(j = 0; j < n; j++)
                {
                    D->m[(i+offset)*m+j] = result[i*n+j];
                }
            }
            offset += rows_for_recv;
            free(result);
        }
        show_matrix(D);
        destroy_matrix(A);
        destroy_matrix(B);
        destroy_matrix(C);
        destroy_matrix(D);
    }
    else
    {
        rows_for_recv = (task <= extra) ? rows + 1 : rows;
        tmp = malloc(n*rows_for_recv*sizeof(tipoMatriz));
        result = malloc(n*rows_for_recv*sizeof(tipoMatriz));
        tipoMatriz sum;
        MPI_Recv(tmp, n*rows_for_recv, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1 , MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for(i  = 0; i < rows_for_recv; i++)
        {
            for(j  = 0; j < q; j++)
            {
                sum  = 0;
                for(k = 0; k < n; k++)
                {
                    sum+= tmp[i*(n)+k]*(B->m[k*p+j]);
                }
                result[i*n +j] = sum;
            }
        }
        MPI_Send(result, n*rows_for_recv, MPI_DOUBLE, 0 , 0, MPI_COMM_WORLD);
        free(tmp);
        free(result);
    }
	MPI_Finalize();
	return 0;
}
/*  Matrix: https://software.intel.com/en-us/articles/performance-of-classic-matrix-multiplication-algorithm-on-intel-xeon-phi-processor-system
 *  Note: Allocated memory blocks are not contiguous and can be fragmented by an operating system memory manager, and fragmentation can degrade performance of processing.
 *  SPMD - Single Program, Multiple Data - Tasks are split up and run simultaneously on multiple
 *  processors with different input in order to obtain result faster.
 *  
 *  Foster 1-D matrix data decomposition.
 *      Each task: 
 *              Utilizes subset of cols of A, B, C
 *              Responsible for calculing its C_ij
 *              Requires full copy of A
 *              Requires N*N/P data from each of the other (P-1) tasks
 *              Computation: O((N^3)/P      
 *
 *  Block-Striped Decomposition:
 *      A fine-grained approach - the basic subtask is calculation of one element of C
 *
 *
 * */
