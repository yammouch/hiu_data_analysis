# $ sh compile.sh
# $ ./modPerceptron ../k-means/Hokkaido_xyl.dat 1 0.01
# ^ 2nd arg: a seed of random values, 3rd arg: learning rate

g++ -std=c++11 -o modPerceptron.exe \
 ../../source2016/modPerceptron.cpp \
 ../../source2016/common.cpp
