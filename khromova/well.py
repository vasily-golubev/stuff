import sys
import numpy as np

const = 100.0
x_min = 554400
y_min = 7623300

data = np.loadtxt('well.xyz')
x = data[:, 0] - x_min
y = data[:, 1] - y_min
z = -data[:, 2]

points = len(x)

f = open('./well.vtk', "wb")

# Header
f.write('# vtk DataFile Version 3.0\n')
f.write('well\n')
f.write('ASCII\n')
f.write('DATASET POLYDATA\n')
f.write('POINTS ' + str(points) + ' float\n')
for i in range(points):
	f.write(str(x[i]) + ' ' + str(y[i]) + ' ' + str(z[i]) + '\n')
f.write('VERTICES ' + str(points) + ' ' + str(5 * points) + '\n')
for i in range(points):
	f.write('4 ' + str(i) + ' ' + str(i) + ' ' + str(i) + ' ' + str(i) + '\n')
f.close()

sys.exit(0)
