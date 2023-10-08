#!/bin/bash

for i in $(seq 0 0); do
    echo original
    time ./mandelbrot
    echo extra
    time ./mandelbrot_v2
done