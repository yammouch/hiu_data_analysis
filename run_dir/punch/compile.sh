# $ sh compile.sh
# $ ./genPunch ../../source2016/ratioFruits.dat \
#   ../../source2016/priorPot.dat 50 50 1 > 50_50_1.dat
# $ ./estParam 50_50_1_RatioFruits.dat 50_50_1_PriorPot.dat 50_50_1.dat 4 7

g++ -std=c++11 -o genPunch.exe ../../source2016/genPunch.cpp
g++ -std=c++11 -o estParam.exe ../../source2016/estParam.cpp
