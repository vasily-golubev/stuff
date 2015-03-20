import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as sc_ip

# constants
offset = [70]
n_x  = 100
n_y  = 100
xnew = np.linspace(0, 150, n_x)

# layer_1
x_1_t = np.array([0, 8.1, 14.6, 20, 30, 40.9, 51.1, 60, 66.1, 73.3, 79.5, 86.4, 92.5, 100.9, 107.4, 114.3, 123.1, 130, 135.7, 140.2, 145.5, 150])
y_1_t = np.array([0, 0, 1.1, 2.1, 4.2, 6.1, 8.2, 10, 11.8, 13.7, 15.6, 18, 19.9, 23.1, 25.8,	29.1, 32.9, 35.9, 38.4, 40.1, 41.1, 41.7])
y_1_t = np.array(y_1_t.size * offset) - y_1_t

x_1_b = np.array([0, 7.9, 12.7, 20.1, 29.1, 37.1, 42, 46.4, 50.1, 54.4, 59.7, 64.8, 69.8, 75.5, 79, 80.8, 82.8, 85.2, 87.9, 90.3, 94.2, 98.9, 104.6, 108.5, 115.8, 122.3, 129, 135.2, 138.8, 145.1, 150])
y_1_b = np.array([4.1, 6.1, 7.6, 9.3, 9.5, 8.6, 8.7, 10.1, 12.9, 15.2, 17.3, 18.4, 19.3, 19.1, 18.6, 18.6, 19.5, 20.8, 21.9, 22.3, 23.4, 25.1, 27.1, 28.5, 31.3, 34, 36.8, 39.3, 40.7, 41.9, 42.6])
y_1_b = np.array(y_1_b.size * offset) - y_1_b

f_1_t = sc_ip.interp1d(x_1_t, y_1_t, kind = 'cubic')
f_1_b = sc_ip.interp1d(x_1_b, y_1_b, kind = 'cubic')

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

x_2_b = np.array([0, 10.3, 20.7, 31.1, 41.7, 54.9, 68.5, 78.6, 89.2, 97.9, 105.3, 112.7, 120.1, 127.4, 134.9, 140.8, 145.3, 150])
y_2_b = np.array([34.5, 35.4, 36.5, 36.4, 35.7, 35.8, 37.6, 37.8, 37.8, 38, 38.9, 39.1, 39.9, 40.3, 41.7, 42.8, 43.4, 43.9])
y_2_b = np.array(y_2_b.size * offset) - y_2_b

f_2_t = sc_ip.interp1d(x_2_t, y_2_t, kind = 'cubic')
f_2_b = sc_ip.interp1d(x_2_b, y_2_b, kind = 'cubic')

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

x_3_b = np.array([0, 100, 125, 150])
y_3_b = np.array([50, 50, 50, 50])
y_3_b = np.array(y_3_b.size * offset) - y_3_b

f_3_t = sc_ip.interp1d(x_3_t, y_3_t, kind = 'cubic')
f_3_b = sc_ip.interp1d(x_3_b, y_3_b, kind = 'cubic')

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
#plt.plot(xnew, f_1_t(xnew), '-', xnew, f_1_b(xnew), '-', xnew, f_2_t(xnew), '-', xnew, f_2_b(xnew), '-', xnew, f_3_t(xnew), '-', xnew, f_3_b(xnew), '-')
#plt.show()
