CC=gcc
CFLAG= -Wall -std=c99 -fopenmp -O3 -msse2 -ftree-slp-vectorize -fopt-info-vec-all
LFLAG=  -ftree-vectorizer-verbose=2
NAME=mandelbrot

all: mandelbrot.o mandelbrot_v2.o
	$(CC) $(CFLAG) mandelbrot.o $(LFLAG) -o mandelbrot
	$(CC) $(CFLAG) mandelbrot_v2.o $(LFLAG) -o mandelbrot_v2

mandelbrot.o : mandelbrot.c
	$(CC) -c $(CFLAG) mandelbrot.c $(LFLAG) 

mandelbrot_v2.o : mandelbrot_v2.c
	$(CC) -c $(CFLAG) mandelbrot_v2.c $(LFLAG) 

clean:
	rm -f *.o

purge: clean
	rm -f mandelbrot mandelbrot_v2