FLAGS= -o3

CC=gcc
MPI=mpicc
RM=rm -f

EXEC=tsp
PAR=tsp_par_cut
all: $(EXEC) $(PAR)

$(EXEC):
	$(CC) $(FLAGS) $(EXEC).c -c -lm -o $(EXEC).o
	$(CC) $(FLAGS) $(EXEC).o -lm -o $(EXEC)

$(PAR):
	$(MPI) $(FLAGS) $(PAR).c -lm -o $(PAR)

run:
	mpirun --hostfile host.txt -np 4 ./$(PAR) < tsp1.in

clean:
	$(RM) $(EXEC).o $(EXEC) $(PAR).o $(PAR)