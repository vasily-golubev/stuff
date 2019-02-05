#!/bin/bash
rm main
mpic++ -Wall main.cpp -lm -o main
mpirun -np $1 main
