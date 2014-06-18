#!/bin/bash
gcc -g -Wall -DFIFTH_ORDER -DROUGH_INITIAL advection_ext.c advection_ext.h -lm -o advection_ext
