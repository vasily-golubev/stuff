import math

# XY grid area
x_min = -5000.0
x_max = 5000.0
y_min = -5000.0
y_max = 5000.0

# Points for interpolation
nx = 10
ny = 10

dx = (x_max - x_min) / nx
dy = (y_max - y_min) / ny

# Mean and variation
H = 2000.0
h = 100.0

# Period of variation
T = 4.0 * dx

# Copy-paste in config file
f = open('add_point.txt', 'w')

for i in range (nx + 1):
	for j in range(ny + 1):
		f.write('\t\t\t\t\tadd_point = ' + str(x_min + i * dx) + ', ' + str(y_min + j * dy) + ', ')
		f.write(str(H + h * math.sin(2.0 * math.pi * (i * dx) / T)) + '\n')
f.close()
