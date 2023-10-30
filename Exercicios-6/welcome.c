#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 100
#define STD_TAG 0

int main(int argc, char **argv) {
    int buf_size,i, my_rank, n_procs; 
    char *local_buf;
    char msg[BUF_SIZE];
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    buf_size = BUF_SIZE; local_buf = (char *) malloc(buf_size);
    MPI_Buffer_attach(local_buf, buf_size);
    sprintf(msg, "Welcome!");
    for (i = 0; i < n_procs; i++)
        if (my_rank != i)
            MPI_Ssend(msg, strlen(msg) + 1, MPI_CHAR, i, STD_TAG, MPI_COMM_WORLD);
    for (i = 0; i < n_procs; i++)
        if (my_rank != i) {
            sleep(4);
            sprintf(msg, "Argh!");
            MPI_Recv(msg, 100,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            printf("Proc %d->%d: %s \n", status.MPI_SOURCE, my_rank, msg);
        }

    MPI_Buffer_detach(&local_buf, &buf_size);
    free(local_buf);
    MPI_Finalize();
}