CC=gcc
CFLAG=-Wall -std=c99 -fopenmp -O3
LFLAG=
NAME1=sequencial
NAME2=paralelo

all:$(NAME1) $(NAME2)

$(NAME1): integral_seq.o
	$(CC) $(CFLAG) integral_seq.o $(LFLAG) -o $(NAME1)

$(NAME2): integral_par.o
	$(CC) $(CFLAG) integral_par.o $(LFLAG) -o $(NAME2)

integral_par.o:integral_par.c
	$(CC) -c $(CFLAG) integral_par.c $(LFLAG)

integral_seq.o:integral_seq.c
	$(CC) -c $(CFLAG) integral_seq.c $(LFLAG)

clean:
	rm -f *.o

purge: clean
	rm -f $(NAME1) $(NAME1)