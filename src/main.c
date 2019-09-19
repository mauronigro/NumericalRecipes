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
    int m = atoi(argv[1]), 
        n = atoi(argv[2]), 
        p = atoi(argv[3]), 
        q = atoi(argv[4]);
    if(n != p)
        return 0;
    int task, 
        num_tasks, 
        num_slaves,
        source, 
        dest,
        i,
        j,
        offset,
        extra,
        rows,
        rows_for_send;
    tipoMatriz* tmp;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &task);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Status status;
    struct matrix* A = init_matrix(m, n);
    struct matrix* B = init_matrix(p, q);
    if(task == 0)
    {
        toeplitz(A);
        show_matrix(A);
        num_slaves = num_tasks - 1;
        printf("Numero de Escravos: %d \n", num_slaves);
        rows =  m / num_slaves;
        extra = m % num_slaves;
        offset = 0;
        tipoMatriz* tmp;
        for(dest = 1; dest <= num_slaves; dest++)
        {

            rows_for_send = (dest <= extra) ? rows + 1 : rows;
            tmp = malloc(n*rows_for_send*sizeof(tipoMatriz));
            printf("sending %d rows to task %d \n", rows_for_send, dest);
            MPI_Send(&rows_for_send, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            //printf("Offset:%d \n", offset);
            MPI_Send(&offset, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            for(i = 0; i < n*rows_for_send; i++)
            {
                // atribuir ao vetor temporário as linhas a serem enviadas.
                tmp[i] = A->m[offset*m+i];
                printf("%.0lf ", tmp[i]);
            }
            printf("\n");
            for(i = offset; i < offset + rows_for_send; i++)
            {
                printf("Send for Task: %d, row %d \n", dest, i);
            }
            offset = offset + rows_for_send;
            free(tmp);
        } 
        destroy_matrix(A);
    }
    else
    {
        MPI_Recv(&rows_for_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        //printf("Task: %d Recv %d rows offset %d \n", task, rows_for_send, offset);
        for(i = offset; i < offset+ rows_for_send; i++)
        {
            //printf("Recv for Task %d, row %d \n", task, i);
        }
        //MPI_Recv(B->m, (B->col)*(B->row), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
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
