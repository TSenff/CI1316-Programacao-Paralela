#include <stdio.h>
#include <omp.h>

static long num_step = 50000000;

/*
    Integrando em paralelo
*/



int main(){
    double x, pi, sum = 0.0;
    double step = 1.0/(double)num_step;
    
    for(int i = 0 ; i<num_step;i++){
        x = (i + 0.5) * step;
        sum += 4.0/(1.0+x*x);
    }
    pi = step * sum;
    
    //printf("pi = %lf\n", pi);
    return pi;
}