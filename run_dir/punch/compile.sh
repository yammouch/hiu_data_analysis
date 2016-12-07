# $ sh compile.sh
# $ ./modPerceptron ../k-means/Hokkaido_xyl.dat 1 0.01
# ^ 2nd arg: a seed of random values, 3rd arg: learning rate

g++ -std=c++11 -o genPunch.exe ../../source2016/genPunch.cpp
g++ -std=c++11 -o estParam.exe ../../source2016/estParam.cpp
