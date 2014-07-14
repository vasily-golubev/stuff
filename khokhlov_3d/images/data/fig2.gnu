set terminal pdf size 15cm,15cm
set output "fig2.pdf"

set grid
#set format x ''
#set format y ''
#unset key
set xrange [0:3.464101615]
set yrange [-0.3:1.3]

plot "none.txt" title "analytical" with line lt 1 lc rgb "black" lw 1, \
	"AdvRectSchemaCIR_0.1_000099.txt" title "CIR-3D" with linespoints lt -1 pi 1 pt 5 ps 0.8 lc rgb "black" lw 0, \
	"AdvRectSchemaCIPExt_0.1_000099.txt" title "CIP-3D-1" with linespoints lt -1 pi 1 pt 7 ps 0.8 lc rgb "black" lw 0, \
	"AdvRectSchemaCIPExtCross_0.1_000099.txt" title "CIP-3D-2" with linespoints lt -1 pi 1 pt 9 ps 0.8 lc rgb "black" lw 0
