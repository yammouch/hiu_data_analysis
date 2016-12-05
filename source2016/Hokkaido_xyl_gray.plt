set terminal postscript eps enhanced 
set size ratio -1 0.6
set output "Hokkaido_xyl.eps"
plot [-200:400][-200:300]\
"Hokkaido_xyl.dat" u 1:($3==0 ? $2 : 1/0) title "Cluster0",\
"Hokkaido_xyl.dat" u 1:($3==1 ? $2 : 1/0) title "Cluster1",\
"Hokkaido_xyl.dat" u 1:($3==2 ? $2 : 1/0) title "Cluster2",\
"Hokkaido_xyl.dat" u 1:($3==3 ? $2 : 1/0) title "Cluster3"
