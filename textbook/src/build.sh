#!/bin/bash
gcc -g -Wall -DFOURTH_ORDER -DSMOOTH_INITIAL advection.c advection.h -lm -o advection
