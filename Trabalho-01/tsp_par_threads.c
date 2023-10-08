/* WSCAD - 9th Marathon of Parallel Programming 
 * Simple Brute Force Algorithm for the 
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <omp.h>

double start, end;
double start_middle, end_middle;

int min_distance;
int nb_towns;
int *task_path;

typedef struct {
    int to_town;
    int dist;
} d_info;

d_info **d_matrix;
int *dist_to_origin;

int present (int town, int depth, int *path) {
    int i;
    for (i = 0; i < depth; i++)
        if (path[i] == town) return 1;
    return 0;
}

void tsp_rec (int depth, int current_length, int *path) {
    int i;
    if (current_length >= min_distance) return;
    if (depth == nb_towns) {
        current_length += dist_to_origin[path[nb_towns - 1]];
        /* Peguei essa ideia da nossa roda de conversa durante a aula */
        if (current_length < min_distance)
            #pragma omp critical
            if (current_length < min_distance)
                min_distance = current_length;

    } else {
        int town, me, dist;
        me = path[depth - 1];
        for (i = 0; i < nb_towns; i++) {
            town = d_matrix[me][i].to_town;
            if (!present (town, depth, path)) {
                path[depth] = town;
                dist = d_matrix[me][i].dist;
                tsp_rec (depth + 1, current_length + dist, path);
            }
        }
    }
}

void tsp (int depth, int current_length, int *path) {
    int town, me, dist;
    me = path[depth - 1];
    /* Desce até o nivel que será paralelizado */
    if(depth < 2){
        for (int i = 0; i < nb_towns; i++) {
            town = d_matrix[me][i].to_town;
            if (!present (town, depth, path)) {
                path[depth] = town;
                dist = d_matrix[me][i].dist;
                tsp (depth + 1, current_length + dist, path);
            }
        }
    } 
    else{
        /* Cria tasks no nivel sendo paralelizado*/
        for (int i = 0; i < nb_towns; i++) {
            town = d_matrix[me][i].to_town;
            if (!present (town, depth, path)) {
                path[depth] = town;
                dist = d_matrix[me][i].dist;
                task_path = (int*) malloc(sizeof(int) * nb_towns);
                memcpy(task_path,path,sizeof(int) * nb_towns);
                #pragma omp task default(none) firstprivate(task_path,current_length,dist,depth) shared(min_distance,nb_towns, d_matrix, dist_to_origin)
                {
                    tsp_rec (depth + 1, current_length + dist, task_path);
                    free(task_path);
                }
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

void init_tsp() {
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;

    st = scanf("%u", &nb_towns);
    if (st != 1) exit(1);
 
    d_matrix = (d_info**) malloc (sizeof(d_info*) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info*) malloc (sizeof(d_info) * nb_towns);
    dist_to_origin = (int*) malloc(sizeof(int) * nb_towns);
   
    x = (int*) malloc(sizeof(int) * nb_towns);
    y = (int*) malloc(sizeof(int) * nb_towns);
    

    for (i = 0; i < nb_towns; i++) {
        st = scanf("%u %u", x + i, y + i);
        if (st != 2) exit(1);
    }
    
    greedy_shortest_first_heuristic(x, y);
    
    free(x);
    free(y);
}

int run_tsp() {
    int i, *path;

    init_tsp();
    
    path = (int*) malloc(sizeof(int) * nb_towns);
    path[0] = 0;
    
    /*Finaliza tempo não paralelizavel inicial / começo da area paralelizavel*/
    start_middle = omp_get_wtime();
        #pragma omp parallel shared(min_distance,nb_towns, d_matrix, dist_to_origin) private(task_path) 
        {
            #pragma omp single
            {
                tsp (1, 0, path);
            }
        }
    /*Inicia tempo não paralelizavel final / fim da area paralelizavel*/
    end_middle = omp_get_wtime();

    free(path);
    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);
    free(d_matrix);

    return min_distance;
}

int main (int argc, char **argv) {
    /*Tempo inicial do alg*/
    start = omp_get_wtime();
    
    int resultado = run_tsp();
    
    /* Finaliza tempo total*/
    end = omp_get_wtime();

    /* Valor alcançado*/
    printf("%d\n", resultado);
    /* Tempo total*/
    printf("%f\n", end - start);
    /* Tempo paralelizado*/
    printf("%f\n", (end_middle-start_middle));
    return 0;
}
