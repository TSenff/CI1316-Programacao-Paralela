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

clock_t start, end;
clock_t start_middle, end_middle;

int min_distance;
int nb_towns;

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
        if (current_length < min_distance)
            min_distance = current_length;
    } else {
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
    
    present = calloc(nb_towns,sizeof(char));
    present[0] = 1;

    free(x);
    free(y);
}

int run_tsp() {
    int i;

    init_tsp();
    
    
    /*Finaliza tempo não paralelizavel inicial / começo da area paralelizavel*/
    start_middle = clock();

    tsp (1, 0, 0);

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
    
    int resultado = run_tsp();
    
    /* Finaliza tempo total*/
    end = clock();

    /* Valor alcançado*/
    printf("%d\n", resultado);

    /* Tempo total*/
    printf("%f\n", (float)(end - start) / CLOCKS_PER_SEC );
    /* Tempo não paralelizado*/
    printf("%f\n", (float)(start_middle - start) / CLOCKS_PER_SEC +  (float)(end - end_middle) / CLOCKS_PER_SEC );
    return 0;
}