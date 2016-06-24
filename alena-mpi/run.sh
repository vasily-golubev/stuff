#!/bin/bash
rm main
mpic++ main.cpp -o main
mpirun -np $1 main
