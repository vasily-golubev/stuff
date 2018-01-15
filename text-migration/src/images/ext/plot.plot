set terminal png size 600,500
set output "fig_compact_cmp_rough.png"
set xlabel "Coordinate"
set ylabel "Value"
plot "plot_3rd.dat" using 1:2 lt -1 pt 4 title "3rd", \
x < -0.5 ? 0 : x < 0.5 ? 1 : 0 lt -1 title "init"
#"plot_5th.dat" using 1:2 lt -1 pt 6 title "5th", \
