#!/usr/bin/python

import sys
import matplotlib.pyplot as pp
import numpy as np

if len(sys.argv) < 5:
	print "syntax: calc_displacement_vtk.py vtk/single_point_water_ steps dt dx==dy"
	sys.exit(1)
path = sys.argv[1]
steps = int(sys.argv[2])
dt = float(sys.argv[3])
dxdy = int(sys.argv[4])
disp = np.zeros((dxdy, dxdy))
max_t = np.zeros((dxdy, dxdy))
# Assume that v(0) = 0 mm/s
for i in range(0, steps, 10): #FIXME
	fin = file(path + str(i).zfill(6) + ".vtk", "r")
	print "Processing " + str(fin) + "..."
	for i in range(10):
		fin.readline()
	v = np.fromfile(fin, dtype = np.dtype(">f4"), count = dxdy ** 2).reshape(dxdy, dxdy)
	for i in range(3):
		fin.readline()
	v = np.fromfile(fin, dtype = np.dtype(">f4"), count = dxdy ** 2).reshape(dxdy, dxdy)
	for i in range(3):
		fin.readline()
	v = np.fromfile(fin, dtype = np.dtype(">f4"), count = dxdy ** 2).reshape(dxdy, dxdy)
	disp = disp + v * dt * 1e3
	for i in range(dxdy):
		for j in range(dxdy):
			if abs(disp[i, j]) > abs(max_t[i, j]):
				max_t[i, j] = disp[i, j]
pp.imshow(np.flipud(disp), extent = [-2000, 2000, -2000, 2000])
pp.colorbar().set_label("Displacement, mm")
pp.xlabel("X-axis, m")
pp.ylabel("Y-axis, m")
pp.show()
