# $ sh compile.sh
# $ ./k-means ../../source2016/HokkaidoTowns_xy.dat 6 1
# ^ 2nd arg: the number of clusters, 3rd arg: a seed of random values
# $ gnuplot Hokkaido_xyl_gray.plt
# $ ps2pdf Hokkaido_xyl_gray.eps

g++ -std=c++11 -o k-means.exe \
 ../../source2016/k-means.cpp \
 ../../source2016/common.cpp
