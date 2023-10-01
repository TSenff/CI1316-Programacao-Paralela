FLAGS=

CC=gcc

RM=rm -f

EXEC=tsp
PAR=tsp_par
all: $(EXEC) $(PAR)

$(EXEC):
	$(CC) $(FLAGS) $(EXEC).c -c -lm -o $(EXEC).o
	$(CC) $(FLAGS) $(EXEC).o -lm -o $(EXEC)

$(PAR):
	$(CC) $(FLAGS) $(PAR).c -c -lm -o $(PAR).o
	$(CC) $(FLAGS) $(PAR).o -lm -o $(PAR)

run:
	./teste.sh

clean:
	$(RM) $(EXEC).o $(EXEC) $(PAR).o $(PAR)
