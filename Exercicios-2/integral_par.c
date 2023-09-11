#include <stdio.h>
#include <omp.h>

static long num_step = 50000000;

/*
    Integrando em paralelo, usando metodos mais avancados da omp
*/

int main(){
    double pi, sum = 0.0;
    double step = 1.0/(double)num_step;

    #pragma omp parallel for reduction (+:sum)
    for(int i=0 ; i<num_step ; i++){
            double x = (i + 0.5) * step;
            sum += 4.0/(1.0+x*x);
    }
    
    pi = step * sum;
    
    printf("pi = %lf\n", pi);
    return pi;
}