set terminal png
set output "plot.png"
set xlabel "Coordinate"
set ylabel "Value"
plot "plot.dat" using 1:2 title "Solution"
