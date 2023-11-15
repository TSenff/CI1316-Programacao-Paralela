/* WSCAD - 9th Marathon of Parallel Programming 
 * Simple Brute Force Algorithm for the 
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

clock_t start, end;
clock_t start_middle, end_middle;

int nb_towns, min_distance;
int global_update_counter, global_min_distance;

#define UPDATE_INTERVAL 500

typedef struct {
    int to_town;
    int dist;
} d_info;

d_info **d_matrix;
int *dist_to_origin;
char *present;

void tsp (int depth, int current_length, int last_town) {
    int i;
    if (current_length >= min_distance) 
        return;
    if (depth == nb_towns) {
        current_length += dist_to_origin[last_town];
        if (current_length < min_distance){
            min_distance = current_length;
        }

        if (global_update_counter == UPDATE_INTERVAL){
            global_update_counter = -1;
        }
        global_update_counter++;

    }
    else {
        int town, dist;
        for (i = 0; i < nb_towns; i++) {
            town = d_matrix[last_town][i].to_town;
            if (!present[town]) {
                dist = d_matrix[last_town][i].dist;
                present[town] = 1;
                tsp (depth + 1, current_length + dist, town);
                present[town] = 0;
            }
        }
    }
}

void greedy_shortest_first_heuristic(int *x, int *y) {
    int i, j, k, dist;
    int *tempdist;

    tempdist = (int*) malloc(sizeof(int) * nb_towns);
    //Could be faster, albeit not as didactic.
    //Anyway, for tractable sizes of the problem it
    //runs almost instantaneously.
    for (i = 0; i < nb_towns; i++) {
        for (j = 0; j < nb_towns; j++) {
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            tempdist [j] = dx * dx + dy * dy;
        }
        for (j = 0; j < nb_towns; j++) {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++) {
                if (tempdist [k] < tmp) {
                    tmp = tempdist [k];
                    town = k;
                }
            }
            tempdist [town] = INT_MAX;
            d_matrix[i][j].to_town = town;
            dist = (int) sqrt (tmp);
            d_matrix[i][j].dist = dist;
            if (i == 0)
                dist_to_origin[town] = dist;
        }
    }

    free(tempdist);
}

void init_tsp(int my_rank, int n_procs) {
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;

    if(!my_rank){
        st = scanf("%u", &nb_towns);
        if (st != 1) exit(1);
    }

    MPI_Bcast(&nb_towns,1,MPI_INT,0,MPI_COMM_WORLD);

    d_matrix = (d_info**) malloc (sizeof(d_info*) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info*) malloc (sizeof(d_info) * nb_towns);
    dist_to_origin = (int*) malloc(sizeof(int) * nb_towns);
   
    x = (int*) malloc(sizeof(int) * nb_towns);
    y = (int*) malloc(sizeof(int) * nb_towns);
    
    if(!my_rank){
        for (i = 0; i < nb_towns; i++) {
            st = scanf("%u %u", x + i, y + i);
            if (st != 2) exit(1);
        }
    }

    MPI_Bcast(x,nb_towns,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(y,nb_towns,MPI_INT,0,MPI_COMM_WORLD);


    greedy_shortest_first_heuristic(x, y);
    

    free(x);
    free(y);
}

int run_tsp(int my_rank, int n_procs) {
    int i, town;

    init_tsp(my_rank,n_procs);

    present = calloc(nb_towns,sizeof(char));
    present[0] = 1;
    
    /*Finaliza tempo não paralelizavel inicial / começo da area paralelizavel*/
    start_middle = clock();

    int block_size = nb_towns/n_procs;
    int residue    = nb_towns%n_procs;
    int block_start = block_size*my_rank + 1;
    int block_end = block_start + block_size;
    printf("My rank is %d, I start at %d and end at %d\n",my_rank,block_start,block_end);

    if(my_rank < (residue-1))
        block_end++;
    for(i = block_start; i < block_end; i++ ){
        town = d_matrix[0][i].to_town;
        present[town] = 1;
        tsp (2,d_matrix[0][i].dist, town);
        present[town] = 0;
    }

    MPI_Allreduce(&min_distance, &min_distance, 1,MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    /*Inicia tempo não paralelizavel final / fim da area paralelizavel*/
    end_middle = clock();

    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);
    free(d_matrix);

    return min_distance;
}

int main (int argc, char **argv) {
    /*Tempo inicial do alg*/
    start = clock();
    
    int my_rank, n_procs;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

    int resultado = run_tsp(my_rank, n_procs);
    /* Valor alcançado*/
    printf("%d\n", resultado);
    /* Finaliza tempo total*/
    MPI_Finalize();

    end = clock();



    /* Tempo total*/
    printf("%f\n", (float)(end - start) / CLOCKS_PER_SEC );
    /* Tempo não paralelizado*/
    printf("%f\n", (float)(start_middle - start) / CLOCKS_PER_SEC +  (float)(end - end_middle) / CLOCKS_PER_SEC );
    return 0;
}