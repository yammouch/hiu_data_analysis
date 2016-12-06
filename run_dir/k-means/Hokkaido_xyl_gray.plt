set terminal postscript eps enhanced 
set size ratio -1 0.6
set output "Hokkaido_xyl.eps"
# for seed of 1
#plot [-200:400][-200:300]\
#"Hokkaido_xyl.dat" u 1:($3==0 ? $2 : 1/0) with points pt 6 title "Cluster0",\
#"Hokkaido_xyl.dat" u 1:($3==1 ? $2 : 1/0) with points pt 1 title "Cluster1",\
#"Hokkaido_xyl.dat" u 1:($3==2 ? $2 : 1/0) with points pt 2 title "Cluster2",\
#"Hokkaido_xyl.dat" u 1:($3==3 ? $2 : 1/0) with points pt 3 title "Cluster3",\
#"Hokkaido_xyl.dat" u 1:($3==4 ? $2 : 1/0) with points pt 4 title "Cluster4",\
#"Hokkaido_xyl.dat" u 1:($3==5 ? $2 : 1/0) with points pt 5 title "Cluster5"
# for seed of 4
plot [-200:400][-200:300]\
"Hokkaido_xyl.dat" u 1:($3==3 ? $2 : 1/0) with points pt 6 title "Cluster0",\
"Hokkaido_xyl.dat" u 1:($3==1 ? $2 : 1/0) with points pt 1 title "Cluster1",\
"Hokkaido_xyl.dat" u 1:($3==2 ? $2 : 1/0) with points pt 2 title "Cluster2",\
"Hokkaido_xyl.dat" u 1:($3==4 ? $2 : 1/0) with points pt 3 title "Cluster3",\
"Hokkaido_xyl.dat" u 1:($3==0 ? $2 : 1/0) with points pt 4 title "Cluster4",\
"Hokkaido_xyl.dat" u 1:($3==5 ? $2 : 1/0) with points pt 5 title "Cluster5"
