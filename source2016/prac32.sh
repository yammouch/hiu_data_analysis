for seed in `seq 0 999`
do
 ./randomLabel HokkaidoCities_xy.dat 4 $seed | ./calcJw
done
