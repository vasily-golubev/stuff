set terminal png size 600,500
set output "fig_wide_stencil_cmp_rough.png"
set xlabel "Coordinate"
set ylabel "Value"
plot "plot_1st.dat" using 1:2 lt -1 pt 4 title "1st", \
"plot_2nd.dat" using 1:2 lt -1 pt 6 title "2nd", \
"plot_3rd.dat" using 1:2 lt -1 pt 8 title "3rd", \
x < -0.5 ? 0 : x < 0.5 ? 1 : 0 lt -1 title "init"
