set terminal postscript eps enhanced 
set size 0.6,0.6
set output "punch_pot.eps"
set logscale x
set logscale y
#set format x "%2.1t {/Symbol \264} 10^{%T}"
set grid
plot [10:10000][1e-3:1]\
"punch_pot.dat" u 1:2 with linespoints pt 1 title "pot",\
"punch_pot.dat" u 1:3 with linespoints pt 2 title "fruit"
