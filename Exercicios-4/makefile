CC=gcc
CFLAG=-Wall -std=c99 -fopenmp -O3
LFLAG=
NAME1=linked
NAME2=linked_task

all: linked.o linked_task.o
	$(CC) $(CFLAG) linked.o $(LFLAG) -o $(NAME1)
	$(CC) $(CFLAG) linked_task.o $(LFLAG) -o $(NAME2)

linked.o :linked.c
	$(CC) -c $(CFLAG) linked.c $(LFLAG)

linked_task.o: linked_task.c
	$(CC) -c $(CFLAG) linked_task.c $(LFLAG)

clean:
	rm -f *.o

purge: clean
	rm -f $(NAME1) $(NAME2)