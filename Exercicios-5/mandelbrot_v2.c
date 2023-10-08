/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**            
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with 
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 10000
# define MAXITER 1000


struct d_complex{
   double r;
   double i;
};

void testpoint(struct d_complex c);

int numoutside = 0;

int main(){
  int i, j;
  double area, error, eps  = 1.0e-5;
  struct d_complex c;
  //   Loop over grid of points in the complex plane which contains the Mandelbrot set,
  //   testing each point to see whether it is inside or outside the set.
  #pragma omp parallel default(none) private(c) firstprivate(eps)
  {
      #pragma omp single
      {
        #pragma omp taskloop collapse(2)
        for (i=0; i<NPOINTS; i++) {
          for (j=0; j<NPOINTS; j++) {
            c.r = -2.0+2.5*(double)(i)/(double)(NPOINTS)+eps;
            c.i = 1.125*(double)(j)/(double)(NPOINTS)+eps;
            testpoint(c);
          }
        }
      }
  }

  // Calculate area of set and error estimate and output the results
  area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
  error=area/(double)NPOINTS;
  printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
  printf("Correct answer should be around 1.510659\n");

}

void testpoint(struct d_complex c){
  // Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
  // If loop count reaches MAXITER, point is considered to be inside the set
  struct d_complex z;
  int iter;
  double zr_2, zi_2;
  z=c;
  for (iter=0; iter<MAXITER; iter++){
    zr_2 = z.r*z.r;       
    zi_2 = z.i*z.i;       

    z.i = z.r*z.i*2+c.i;
    z.r = zr_2-zi_2+c.r;
    if ((zr_2+zi_2)>4.0) {
      #pragma omp critical
      numoutside++;
      break;
    }
  }

}

