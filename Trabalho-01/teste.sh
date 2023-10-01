#!/bin/bash

for i in $(seq 0 2); do
    echo "------------------------------"
    echo "tsp original"
    time ./tsp < tsp.in
    echo "tsp paralelo"
    time ./tsp_par  < tsp.in
    echo "------------------------------"
done