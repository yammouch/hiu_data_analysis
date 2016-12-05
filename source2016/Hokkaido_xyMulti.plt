set terminal postscript eps enhanced 
set size ratio -1
set output "Hokkaido_xyMulti.eps"
set xrange [-200:400]
set yrange [-200:300]
set multiplot layout 1,2
plot "HokkaidoTowns_xy.dat" pt 6 title "Before"
plot "HokkaidoTowns_xy2.dat" u 2:1 pt 6 title "After transform"
