import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import scipy.interpolate as sc_ip

# constants
sources = ['./data/Area_1_A3_2.dat', './data/Area_1_Acb_5.dat', './data/Area_1_C2_10.dat', './data/Area_1_J2_1.dat', './data/Area_1_P1AR_II_8.dat', './data/Area_1_Aan_3.dat', './data/Area_1_Ahr_4.dat', './data/Area_1_Iu_P2_6.dat', './data/Area_1_P1AR_I_7.dat', './data/Area_1_P1S_III_9.dat']

fig = plt.figure()
ax = fig.add_subplot(111, projection = '3d')
for source in sources:
	data = np.genfromtxt(source)
	x = data[:, 0]
	y = data[:, 1]
	print source
	z = -data[:, 2]

	nx = 20
	ny = 20
	if source == './data/Area_1_Acb_5.dat': # Can't do with nx=ny=20
		nx = 10
		ny = 10
	print x.max() - x.min()
	print y.max() - y.min()
	xi = np.linspace(x.min(), x.max(), nx)
	yi = np.linspace(y.min(), y.max(), ny)
	zi = sc_ip.griddata((x, y), z, (xi[None, :], yi[:, None]), method = 'cubic')
	xig, yig = np.meshgrid(xi, yi)

	ax.plot_wireframe(xig, yig, zi)

plt.show()

sys.exit(0)
