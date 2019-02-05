#!/usr/bin/python

# READ http://archives.caltech.edu/pictures/1.46.1.-17.jpg to see Richter scale

import sys
import matplotlib.pyplot as pp

if len(sys.argv) < 3:
	print "syntax: calc_displacement.py vtk/single_point_water_ steps dt "
	sys.exit(1)
time = []
vy = []
path = sys.argv[1]
steps = int(sys.argv[2])
dt = float(sys.argv[3])
for i in range(steps):
	fin = open(path + str(i).zfill(6) + ".vtk", "r")
	# HACK skip comments in first file
	if i == 0:
		fin.readline()
	data = fin.readline().split("\t")
	time.append(data[0])
	vy.append(data[3])
# Assume that v(0) = 0 mm/s
disp = [0]
for vel in vy:
	disp.append(disp[-1] + float(vel) * dt * 1e3)
pp.plot(time, disp[:-1])
pp.xlabel("Time, s")
pp.ylabel("Displacement, mm")
pp.show()
