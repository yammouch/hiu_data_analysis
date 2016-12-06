set terminal postscript eps enhanced 
set size ratio -1 0.6
set output "compLearn.eps"
set logscale x
set format x "10^{%T}"
#set format y "%2.1f {/Symbol \264} 10^{5}"
set format y "%2.1t {/Symbol \264} 10^{%T}"
set grid
#plot [5e-6:1][4.5:7]\
#"compLearn.dat" u 1:($2/100000) with linespoints pt 1 title "E_Q",\
#"compLearn.dat" u 1:($3/100000) with linespoints pt 2 title "J_W"
plot [5e-6:1][4.5e5:7e5]\
"compLearn.dat" u 1:2 with linespoints pt 1 title "E_Q",\
"compLearn.dat" u 1:3 with linespoints pt 2 title "J_W"
