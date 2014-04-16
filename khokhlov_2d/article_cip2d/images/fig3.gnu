set terminal pdf size 15cm,15cm
set output "fig3.pdf"

set grid
unset key
set format x ''
set format y ''

# TODO Good enough?
set xrange [-1:1]
set yrange [-1:1]
set zrange [-0.3:1.3]
unset ztics
set view 60,15

set multiplot layout 2,2 rowsfirst

set label 1 'а)' at graph 0.1,-0.2 font ',12'
splot "BIS1-1.txt" with line lt 1 lc rgb "black" lw 1

set label 1 'б)' at graph 0.1,-0.2 font ',12'
splot "BIS1-2.txt" with line lt 1 lc rgb "black" lw 1

set label 1 'в)' at graph 0.1,-0.2 font ',12'
splot "BIS2-1.txt" with line lt 1 lc rgb "black" lw 1

set label 1 'г)' at graph 0.1,-0.2 font ',12'
splot "BIS2-2.txt" with line lt 1 lc rgb "black" lw 1

unset multiplot

set output
