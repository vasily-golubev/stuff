set terminal pdf size 15cm,15cm
set output "../fig1_3d.pdf"

set grid
set format x ''
set format y ''
unset key

set yrange [-0.3:1.3]

set multiplot layout 3,2 rowsfirst

set label 1 'а)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
	"3d/AdvRectSchemaCIPExt_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0

set label 1 'б)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
	"3d/AdvRectSchemaCIPExtCross_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0

set label 1 'в)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
	"3d/AdvRectSchemaCIPMMExt_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0

set label 1 'г)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
    "3d/AdvRectSchemaCIPMMExtCross_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0

set label 1 'д)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
    "3d/AdvRectSchemaCIP23Ext_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0

set label 1 'е)' at graph 0.92,0.9 font ',12'
plot "3d/none.txt" with line lt 1 lc rgb "black" lw 1, \
    "3d/AdvRectSchemaCIP23ExtCross_0.1_000099.txt" with linespoints lt -1 pi 1 pt 7 ps 0.4 lc rgb "black" lw 0


unset multiplot
