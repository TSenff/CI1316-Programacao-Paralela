#!/bin/bash

for i in $(seq 0 0); do
    echo IIIIIIIIIIIIIIII Teste $i IIIIIIIIIIIIIIII
    echo paralelo
    time ./paralelo
    echo sequencial
    time ./sequencial
done