set terminal postscript eps enhanced 
set size ratio 1 0.6
set output "data2D.eps"
plot [0:5][0:5] "data2D.dat"
