import sys
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as sc_ip

# constants
#sources = ['Area_1_J2_1', 'Area_1_A3_2', 'Area_1_Aan_3', 'Area_1_Ahr_4', 'Area_1_Iu_P2_6', 'Area_1_P1AR_I_7', 'Area_1_P1AR_II_8', 'Area_1_P1S_III_9', 'Area_1_C2_10', 'Area_1_Acb_5']
sources = ['Area_1_J2_1', 'Area_1_A3_2']
#! manual calculation
x_min = 554400
x_max = 558000
y_min = 7623300
y_max = 7626700

# common stuff
nx = 100
ny = 100
xi = np.linspace(x_min, x_max, nx)
yi = np.linspace(y_min, y_max, ny)

# old profile for mesh generation
h_prev = []

for source in sources:
	if source == 'Area_1_Acb_5': # Can't interpolate 100 x 100
		continue
	if source == 'Area_1_P1AR_I_7' or source == 'Area_1_P1AR_II_8': # A first approach - skip
		continue
	data = np.genfromtxt('./data/' + source + '.dat')
	X = data[:, 0]
	Y = data[:, 1]
	print source # Progress ...
	Z = -data[:, 2]
	zi = sc_ip.griddata((X, Y), Z, (xi[None, :], yi[:, None]), method = 'cubic')
	h = zi

	# first profile processing
	if len(h_prev) == 0:
		h_prev = h
		continue

	# create mesh
	NX = nx
	NY = ny
	NZ = 100 # for each layers
	f = open('./3dgrids/' + source + '.grd', "wb")
	for i in range(NX):
		for j in range(NY):
			zi = np.linspace(h[i][j], h_prev[i][j], NZ)
			for k in range(NZ):
				f.write(xi[i])
				f.write(yi[j])
				f.write(zi[k])
	f.close()

	h_prev = h

sys.exit(0)
