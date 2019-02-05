import sys
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as sc_ip

# constants
sources = ['Area_2_J2_1',
	'Area_2_A3_2',
	'Area_2_Aan_3',
	'Area_2_Ahr_4',
	'Area_2_Acb_5',
	'Area_2_Iu_P2_6',
	'Area_2_P1AR_I_7',
	'Area_2_P1AR_II_8',
	'Area_2_P1S_III_9',
	'Area_2_C2_10']
#! manual calculation
x_min = 557000
x_max = 560000
y_min = 7618000
y_max = 7621000

# common stuff
nx = 50
ny = 50
xi = np.linspace(x_min, x_max, nx)
yi = np.linspace(y_min, y_max, ny)
y_center = ny / 2

# old profile for mesh generation
h_prev = []

for source in sources:
	if source == 'Area_1_Acb_5' or source == 'Area_1_Iu_P2_6': # Inga told eliminate now
		continue
	#if source == 'Area_1_Acb_5': # Can't interpolate 100 x 100
	#	continue
	if source == 'Area_1_P1AR_II_8': # A first approach - skip
		continue
	data = np.genfromtxt('./data/' + source + '.dat')
	X = data[:, 0]
	Y = data[:, 1]
	print source # Progress ...
	Z = -data[:, 2]
	zi = sc_ip.griddata((X, Y), Z, (xi[None, :], yi[:, None]), method = 'cubic')
	h = zi[:, y_center]
	# draw me
	line, = plt.plot(xi - x_min, h)
	line.set_label(source)
	plt.legend()

	# first profile processing
	if len(h_prev) == 0:
		h_prev = h
		continue

	print min(abs(h - h_prev))
	# create mesh
	NX = nx
	NY = 10 # for each layer
	f = open('./grids/' + source + '.red', "w")
	# header
	f.write(str(NX) + ' ' + str(NY) + '\n')
	f.write(source + '.red\n')
	# 1 edge
	for i in range(len(xi)):
		f.write(str(xi[i] - x_min) + " " + str(h[i]) + " ")
	f.write("\n")
	# 2 edge
	x = xi[xi.size - 1]
	ynew = np.linspace(h[xi.size - 1], h_prev[xi.size - 1], NY)
	for y in ynew[1:]:
		f.write(str(x - x_min) + " " + str(y) + " ")
	f.write("\n")
	# 3 edge
	for i in range(len(xi) - 2, -1, -1):
		f.write(str(xi[i] - x_min) + " " + str(h_prev[i]) + " ")
	f.write("\n")
	# 4 edge
	x = xi[0]
	ynew = np.linspace(h_prev[0], h[0], NY)
	for y in ynew[1:-1]:
		f.write(str(x - x_min) + " " + str(y) + " ")
	f.write("\n")
	# inner nodes
	for i in range(1, len(xi) - 1):
		ynew = np.linspace(h[i], h_prev[i], NY)
		for y in ynew[1:-1]:
			f.write(str(xi[i] - x_min) + " " + str(y) + " ")
	f.close()

	h_prev = h
plt.show()

sys.exit(0)
