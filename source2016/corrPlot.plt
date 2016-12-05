set terminal postscript eps enhanced 
set size ratio -1 0.6
set output "corrPlot.eps"
set nokey
plot [-4.5:4.5][-4.5:4.5]\
"corr.dat" u 3:4 pt 6
