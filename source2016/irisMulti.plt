set terminal postscript eps enhanced 
set size ratio -1
set output "irisMulti.eps"
set multiplot layout 2,1
set key outside
plot \
"pcaIris.dat" u 4:(stringcolumn(5) eq "setosa" ? $3 : 1/0) \
title "setosa",\
"pcaIris.dat" u 4:(stringcolumn(5) eq "versicolor" ? $3 : 1/0) \
title "versicolor",\
"pcaIris.dat" u 4:(stringcolumn(5) eq "virginica" ? $3 : 1/0) \
title "virginica"
plot \
"discrimIris.dat" u 2:(stringcolumn(3) eq "setosa" ? $1 : 1/0) \
title "setosa",\
"discrimIris.dat" u 2:(stringcolumn(3) eq "versicolor" ? $1 : 1/0) \
title "versicolor",\
"discrimIris.dat" u 2:(stringcolumn(3) eq "virginica" ? $1 : 1/0) \
title "virginica"
