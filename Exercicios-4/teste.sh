#!/bin/bash

for i in $(seq 0 0); do
    echo No task
    ./linked
    echo With task
    ./linked _task
done