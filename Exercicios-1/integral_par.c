#include <stdio.h>
#include <omp.h>

static long num_step = 100000;
static int  max_thr  = 4;

/*
    Integrando em paralelo
    0 3 
    2 3 4 5
*/



int main(){
    double x, pi, integral = 0.0;
    double array[4];
    array[0] = 0.0;
    array[1] = 0.0;
    array[2] = 0.0;
    array[3] = 0.0;
    double step = 1.0/(double)num_step;
    

    #pragma omp parallel
    {
        double sum = 0.0;
        for(int i=omp_get_thread_num() ; i<num_step ; i+=max_thr){
                x = (i + 0.5) * step;
                sum += 4.0/(1.0+x*x);
        }

        array[omp_get_thread_num()] = sum;
    }
    
    for(int i=0 ; i<4;i++)
        integral += array[i];

    pi = step * integral;
    
    //printf("pi = %lf\n", );
    return pi;
}