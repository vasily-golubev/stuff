#!/bin/bash
gcc -g -Wall -DSECOND_ORDER -DSMOOTH_INITIAL advection.c advection.h -lm -o advection
