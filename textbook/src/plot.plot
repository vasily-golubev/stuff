set terminal png
set output "plot.png"
set xlabel "Coordinate"
set ylabel "Value"
plot "plot.dat" using 1:2 title "1 full rotation", \
sin(3.14*x)*sin(3.14*x)*sin(3.14*x)*sin(3.14*x) title "analytical"
