#!/usr/bin/python
data_cubic = []
data_curve = []
lines = file("./cubic.csv", "r").readlines()
for line in lines:
	data_cubic.append(line.strip().split(";")[1:])
lines = file("./curve.csv", "r").readlines()
for line in lines:
	data_curve.append(line.strip().split(";")[1:])
max = 0
for i in range(1, 101, 1):
	val = abs(float(data_cubic[i][150 * 3 + 2]))
	if val > max:
		max = val
print max
max_cubic = 0
max_curve = 0
for i in range(600, 901, 1):
	for j in range(135 * 3, 135 * 3 + 31 * 3, 3):
		val = abs(float(data_cubic[i][j + 2]))
		if val > max_cubic:
			max_cubic = val
		val = abs(float(data_curve[i][j + 2]))
		if val > max_curve:
			max_curve = val
print 100.0 * max_cubic / max,"!=", 100.0 * max_curve / max
print 100.0 * (max_cubic - max_curve) / max_cubic
max_cubic = 0
max_curve = 0
for i in range(1000, 1301, 1):
	for j in range(135 * 3, 135 * 3 + 31 * 3, 3):
		val = abs(float(data_cubic[i][j + 2]))
		if val > max_cubic:
			max_cubic = val
		val = abs(float(data_curve[i][j + 2]))
		if val > max_curve:
			max_curve = val
print 100.0 * max_cubic / max,"!=", 100.0 * max_curve / max
print 100.0 * (max_cubic - max_curve) / max_cubic
max_cubic = 0
max_curve = 0
for i in range(1500, 1801, 1):
	for j in range(135 * 3, 135 * 3 + 31 * 3, 3):
		val = abs(float(data_cubic[i][j + 2]))
		if val > max_cubic:
			max_cubic = val
		val = abs(float(data_curve[i][j + 2]))
		if val > max_curve:
			max_curve = val
print 100.0 * max_cubic / max,"!=", 100.0 * max_curve / max
print 100.0 * (max_cubic - max_curve) / max_cubic
