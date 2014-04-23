set terminal pdf size 15cm,15cm
set output "fig2.pdf"

# Margins for each row resp. column
set macros
TMARGIN = "set tmargin at screen 0.9; set bmargin at screen 0.5"
BMARGIN = "set tmargin at screen 0.5; set bmargin at screen 0.1"
LMARGIN = "set lmargin at screen 0.05; set rmargin at screen 0.45"
RMARGIN = "set lmargin at screen 0.55; set rmargin at screen 0.95"

set grid
unset key
set format x ''
set format y ''

# TODO Good enough?
set xrange [-1:1]
set yrange [-1:1]
set zrange [-0.3:1.3]
unset xtics
unset ytics
unset ztics
unset border
set view 60,15

set multiplot layout 2,2 rowsfirst

@TMARGIN; @LMARGIN
set label 1 'а)' at graph 0.1,-0.2 font ',12'
splot "initial.txt" with line lt 1 lc rgb "black" lw 1

@TMARGIN; @RMARGIN
set label 1 'б)' at graph 0.1,-0.2 font ',12'
splot "CIR.txt" with line lt 1 lc rgb "black" lw 1

@BMARGIN; @LMARGIN
set label 1 'в)' at graph 0.1,-0.2 font ',12'
splot "CIP-1.txt" with line lt 1 lc rgb "black" lw 1

@BMARGIN; @RMARGIN
set label 1 'г)' at graph 0.1,-0.2 font ',12'
splot "CIP-2.txt" with line lt 1 lc rgb "black" lw 1

unset multiplot

set output
