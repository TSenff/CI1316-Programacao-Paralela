#!/bin/bash


for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER" >> resultado_sequencial.txt
        ./tsp < $TESTE        >> resultado_sequencial.txt
    done
done

for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER"    >> resultado_paralelo_4_core.txt
        ./tsp_par_4  < $TESTE    >> resultado_paralelo_4_core.txt
    done
done


for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER"  >> resultado_paralelo_3_core.txt
        ./tsp_par_3  < $TESTE  >> resultado_paralelo_3_core.txt
    done
done

for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER"  >> resultado_paralelo_2_core.txt
        ./tsp_par_2  < $TESTE  >> resultado_paralelo_2_core.txt
    done
done

for TESTE in tsp*.in; do
    for ITER in {0..19}; do
        echo "# $TESTE $ITER"    >> resultado_paralelo_1_core.txt
        ./tsp_par_1  < $TESTE    >> resultado_paralelo_1_core.txt
    done
done



