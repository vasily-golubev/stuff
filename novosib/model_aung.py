import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as sc_ip

# constants
offset = [70]
n_x  = 100
n_y  = 100
xnew = np.linspace(0, 200, n_x)

# layer_1
x_1_t = np.array([0, 5.6, 12.9, 19.01, 22.1, 27.9, 32.1, 36.4, 40.1, 43.7, 48.8, 54.7, 57.5, 61.1, 66.4, 69.5, 76.2, 77.4, 80.9, 84.02, 88.6, 93.6, 99.5, 103.9, 109.8, 113.8, 120.4, 125.5, 131.4, 134.9, 137.8, 142.4, 147.3, 156.1, 167.8, 183.1, 193.9, 200])
y_1_t = np.array([0, 0, 1.4, 2.4, 2.4, 3.3, 4.6, 5.2, 5.5, 6.2, 6.7, 7.9, 9.5, 9.5, 11.1, 12.5, 14.7, 16.3, 16.5, 17.5, 18.7, 20.5, 22.9, 24.5, 25.8, 28.3, 31.8, 33.4, 35.4, 38.2, 39.6, 40.3, 41.2, 42.5, 44.2, 46.6, 48.3, 49.5])
y_1_t = np.array(y_1_t.size * offset) - y_1_t

x_1_b = np.array([0, 3.5, 5.96, 8.2, 9.4, 11.3, 13.4, 16.7, 18.4, 21.3, 23.1, 25.3, 27.8, 30.5, 32.9, 34.6, 36.6, 39.6, 42.8, 45.4, 49.8, 52.5, 56.5, 59.9, 65.7, 69.7, 73.2, 76.9, 79.3, 82.01, 83.7, 85.3, 88.5, 94.2, 100.9, 109.9, 117.9, 129.4, 137.8, 151.3, 162.01, 174.04, 181.4, 193.4, 200])
y_1_b = np.array([4.1, 5.2, 6.5, 7.7, 8.4, 9.1, 9.9, 10.3, 10.5, 10.5, 9.9, 8.9, 8.4, 7.6, 7.6, 7.9, 8.9, 10.3, 12.1, 13.9, 15.9, 17.5, 19.1, 21.1, 22.7, 24.7, 25.5, 26.2, 25.2, 24.5, 23.3, 23.04, 23.2, 25.3, 27.4, 30.6, 34.8, 38.4, 41.9, 43.5, 45.9, 47.5, 48.5, 50.7, 50.6])
y_1_b = np.array(y_1_b.size * offset) - y_1_b

f_1_t = sc_ip.interp1d(x_1_t, y_1_t, kind = 'linear')
f_1_b = sc_ip.interp1d(x_1_b, y_1_b, kind = 'linear')

# info for rect SeismogramSaver
print xnew.size
for i in range(0, xnew.size, 1):
	x = xnew[i]
	y = np.linspace(f_1_t(x), f_1_b(x), n_y)[1]
	print str(x) + ", " + str(y) + ",",

#### save
f = open("layer_1.red", "w")
# header
f.write(str(n_x) + " " + str(n_y) + "\n")
f.write("filename.red\n")
# 1 edge
for x in xnew:
	f.write(str(x) + " " + str(f_1_b(x)) + " ")
f.write("\n")
# 2 edge
x = xnew[xnew.size - 1]
ynew = np.linspace(f_1_b(x), f_1_t(x), n_y)
for y in ynew[1:]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# 3 edge
for x in xnew[::-1][1:]:
	f.write(str(x) + " " + str(f_1_t(x)) + " ")
f.write("\n")
# 4 edge
x = xnew[0]
ynew = np.linspace(f_1_t(x), f_1_b(x), n_y)
for y in ynew[1:-1]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# inner nodes
for x in xnew[1:-1]:
	ynew = np.linspace(f_1_b(x), f_1_t(x), n_y)
	for y in ynew[1:-1]:
		f.write(str(x) + " " + str(y) + " ")
f.close()

# layer_2
x_2_t = x_1_b
y_2_t = y_1_b

x_2_b = np.array([0, 6.4, 12.4, 16.1, 22.3, 28.6, 33.9, 37.3, 44.9, 52.4, 58.7, 64.1, 71.4, 76.6, 80.7, 85.3, 92.1, 96.2, 104.8, 112.1, 118.7, 125.7, 131.7, 136.1, 145.04, 151.9, 157.6, 163.9, 169.6, 176.7, 187.96, 200])
y_2_b = np.array([40, 39.8, 39.4, 39.2, 41.2, 41.5, 41.4, 39.6, 40.6, 43.4, 44.9, 45.1, 45.8, 45.4, 44.7, 45.7, 47.1, 44.7, 46.5, 50.1, 49.9, 51.3, 51.1, 50.7, 49.7, 50.7, 51.3, 50.5, 51.7, 51.8, 54.1, 56.5])
y_2_b = np.array(y_2_b.size * offset) - y_2_b

f_2_t = sc_ip.interp1d(x_2_t, y_2_t, kind = 'linear')
f_2_b = sc_ip.interp1d(x_2_b, y_2_b, kind = 'linear')

#### save
f = open("layer_2.red", "w")
# header
f.write(str(n_x) + " " + str(n_y) + "\n")
f.write("filename.red\n")
# 1 edge
for x in xnew:
	f.write(str(x) + " " + str(f_2_b(x)) + " ")
f.write("\n")
# 2 edge
x = xnew[xnew.size - 1]
ynew = np.linspace(f_2_b(x), f_2_t(x), n_y)
for y in ynew[1:]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# 3 edge
for x in xnew[::-1][1:]:
	f.write(str(x) + " " + str(f_2_t(x)) + " ")
f.write("\n")
# 4 edge
x = xnew[0]
ynew = np.linspace(f_2_t(x), f_2_b(x), n_y)
for y in ynew[1:-1]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# inner nodes
for x in xnew[1:-1]:
	ynew = np.linspace(f_2_b(x), f_2_t(x), n_y)
	for y in ynew[1:-1]:
		f.write(str(x) + " " + str(y) + " ")
f.close()

# layer_3
x_3_t = x_2_b
y_3_t = y_2_b

x_3_b = np.array([0, 100, 125, 150, 200])
y_3_b = np.array([70, 70, 70, 70, 70])
y_3_b = np.array(y_3_b.size * offset) - y_3_b

f_3_t = sc_ip.interp1d(x_3_t, y_3_t, kind = 'linear')
f_3_b = sc_ip.interp1d(x_3_b, y_3_b, kind = 'linear')

#### save
f = open("layer_3.red", "w")
# header
f.write(str(n_x) + " " + str(n_y) + "\n")
f.write("filename.red\n")
# 1 edge
for x in xnew:
	f.write(str(x) + " " + str(f_3_b(x)) + " ")
f.write("\n")
# 2 edge
x = xnew[xnew.size - 1]
ynew = np.linspace(f_3_b(x), f_3_t(x), n_y)
for y in ynew[1:]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# 3 edge
for x in xnew[::-1][1:]:
	f.write(str(x) + " " + str(f_3_t(x)) + " ")
f.write("\n")
# 4 edge
x = xnew[0]
ynew = np.linspace(f_3_t(x), f_3_b(x), n_y)
for y in ynew[1:-1]:
	f.write(str(x) + " " + str(y) + " ")
f.write("\n")
# inner nodes
for x in xnew[1:-1]:
	ynew = np.linspace(f_3_b(x), f_3_t(x), n_y)
	for y in ynew[1:-1]:
		f.write(str(x) + " " + str(y) + " ")
f.close()

# visualize
plt.plot(xnew, f_1_t(xnew), '-', xnew, f_1_b(xnew), '-', xnew, f_2_t(xnew), '-', xnew, f_2_b(xnew), '-', xnew, f_3_t(xnew), '-', xnew, f_3_b(xnew), '-')
plt.show()
