set terminal postscript eps enhanced 
set size ratio -1 0.6
set output "modPerceptron.eps"
#set logscale x
set grid
#plot [0.01:1][0:600] "modPerceptron.dat" u 1:2 with linespoints pt 1
plot [0:0.5][0:600] "modPerceptron.dat" u 1:2 with linespoints pt 1 notitle
