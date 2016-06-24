#!/bin/bash
rm main
mpic++ main.cpp -lm -o main
mpirun -np $1 main
