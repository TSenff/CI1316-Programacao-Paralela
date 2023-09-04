#include <stdio.h>
#include <omp.h>

static long num_step = 100000;

/*
    Integrando em paralelo
*/



int main(){
    int i;
    double x, pi, sum = 0.0;
    double step = 1.0/(double)num_step;
    
    for(i=0 ; i<num_step;i++){
        x = (i + 0.5) * step;
        sum += 4.0/(1.0+x*x);
    }
    pi = step * sum;
    
    //printf("pi = %lf\n", pi);
    return pi;
}