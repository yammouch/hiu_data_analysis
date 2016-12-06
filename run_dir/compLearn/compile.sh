# $ sh compile.sh
# $ ./compLearn ../../source2016/HokkaidoTowns_xy.dat 6 1 0.01
# ^ 2nd arg: the number of clusters, 3rd arg: a seed of random values
#   4th arg: learning rate

g++ -std=c++11 -o compLearn.exe \
 ../../source2016/compLearn.cpp \
 ../../source2016/common.cpp
