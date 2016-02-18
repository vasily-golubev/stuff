import sys
import math
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as sc_ip
import struct

# constants
sources = ['Area_1_J2_1',
	'Area_1_A3_2',
	'Area_1_Aan_3',
	'Area_1_Ahr_4',
	'Area_1_Acb_5',
	'Area_1_Iu_P2_6',
	'Area_1_P1AR_I_7',
	'Area_1_P1AR_II_8',
	'Area_1_P1S_III_9',
	'Area_1_C2_10']
#! manual calculation
x_min = 555000
x_max = 558000
y_min = 7623700
y_max = 7626700

# common stuff
nx = 30
ny = 30
nz = 30
xi = np.linspace(x_min, x_max, nx)
yi = np.linspace(y_min, y_max, ny)

# old profile for mesh generation
h_prev = []

# grids for geometry.txt
borders = []

for source in sources:
	#if source == 'Area_1_Acb_5': # Can't interpolate 100 x 100
	#	continue
	if source == 'Area_1_P1AR_II_8': # A first approach - skip
		continue
	data = np.genfromtxt('./data/' + source + '.dat')
	X = data[:, 0]
	Y = data[:, 1]
	print source # Progress ...
	Z = -data[:, 2]
	zi = sc_ip.griddata((X, Y), Z, (xi[None, :], yi[:, None]), method = 'nearest')
	h = zi

	if source == 'Area_1_J2_1': # Day surface case
		h_prev = np.zeros(h.shape)
	if source == 'Area_1_C2_10': # Bottom surface
		h_prev = h
		h =  -2000.0 * np.ones(h.shape)
	borders.append(h)

	# create mesh
	NX = nx - 3
	NY = ny - 3
	if source == 'Area_1_P1S_III_9':
		NZ = 80
	elif source == 'Area_1_Iu_P2_6' :
		NZ = 120
	elif source == 'Area_1_J2_1':
		NZ = 120
	elif source == 'Area_1_Aan_3':
		NZ = 60
	elif source == 'Area_1_C2_10' or source == 'Area_1_Ahr_4' or source == 'Area_1_A3_2':
		NZ = 20
	else:
		print "O!"
	coords = np.zeros((NX, NY, NZ), dtype = ('float64', 3))
	for i in range(NX):
		for j in range(NY):
			if math.isnan(h[i][j]):
				print "NAN. Please, reload me!"
			zi = np.linspace(h[i][j], h_prev[i][j], NZ)
			for k in range(NZ):
				coords[i, j, k][0] = xi[i] - x_min
				coords[i, j, k][1] = yi[j] - y_min
				coords[i, j, k][2] = zi[k]
	f = open('./3dgrids/' + source + '.vtk', "wb")
	# header
	f.write('# vtk DataFile Version 3.0\n')
	f.write('3dmodel\n')
	f.write('BINARY\n')
	f.write('DATASET STRUCTURED_GRID\n')
	f.write('DIMENSIONS ' + str(NX) + ' ' + str(NY) + ' ' + str(NZ) + '\n')
	f.write('POINTS ' + str(NX * NY * NZ) + ' float\n')
	for k in range(NZ):
		for j in range(NY):
			for i in range(NX):
				f.write(struct.pack('>f', coords[i, j, k][0]))
				f.write(struct.pack('>f', coords[i, j, k][1]))
				f.write(struct.pack('>f', coords[i, j, k][2]))
	f.write('POINT_DATA ' + str(NX * NY * NZ) + '\n')
	f.close()

	h_prev = h
# geometry txt generation
ff = open("./geometry.txt", "w")
print borders[8]
for k in range(nz):
        z = -2000.0 + 1.0 * k / (nz - 1) * (0.0 - (-2000.0))
        for j in range(ny):
                for i in range(nz):
                        q = 0
                        while (borders[q + 1][i][j] > z):
				print z, borders[q+1][i][j]
                                q += 1
			ff.write("%s\n" % q)
ff.close()
sys.exit(0)
