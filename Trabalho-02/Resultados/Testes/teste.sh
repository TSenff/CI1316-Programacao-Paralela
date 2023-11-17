#!/bin/bash

echo "## Iniciado em $(date +"%D %T" )"

echo "## Sequencial Inicializado"

for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER" >> resultado_sequencial.txt
        ./tsp < $TESTE        >> resultado_sequencial.txt
    done
done

echo "## Sequencial Completo"

for PROCS in 2 4 8  
do
    echo "## $PROCS Processos Inicializado"
    for TESTE in tsp*.in; do
        for ITER in {0..19}; do
            mpirun --hostfile "host_$PROCS.txt" -np $PROCS ./tsp_par_cut < $TESTE    >> resultado_paralelo_$PROCS.txt
        done
    done
    echo "## $PROCS Processos Completos"
done

echo "## Iniciado em $(date +"%D %T" )"
