#!usr/bin/python
import sys

if len(sys.argv) < 9:
	print "syntax: generate_fractured_cluster.py num width p1_0_x p1_0_y p2_0_x p2_0_y step_x step_y\n"
	sys.exit(1)
num = int(sys.argv[1])
width = sys.argv[2]
p1_0_x = float(sys.argv[3])
p1_0_y = float(sys.argv[4])
p2_0_x = float(sys.argv[5])
p2_0_y = float(sys.argv[6])
step_x = float(sys.argv[7])
step_y = float(sys.argv[8])
fout = open("cracks.conf", "w")
for ind in range(num + 1):
	p1_x = p1_0_x + step_x * ind
	p1_y = p1_0_y + step_y * ind
	p2_x = p2_0_x + step_x * ind
	p2_y = p2_0_y + step_y * ind

	fout.write("\t\t\t\t\t[split]\n")
	fout.write("\t\t\t\t\t\tp1 = ")
	fout.write(str(p1_x) + ", " + str(p1_y) + "\n")
	fout.write("\t\t\t\t\t\tp2 = ")
	fout.write(str(p2_x) + ", " + str(p2_y) + "\n")
	fout.write("\t\t\t\t\t\twidth = ")
	fout.write(str(width) + "\n")
	fout.write("\t\t\t\t\t[/split]\n")
