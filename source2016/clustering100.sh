rm KM4 KM10 CL4 CL10
for seed in `seq 0 99`
do
  ./k-means HokkaidoCities_xy.dat   4 $seed      | ./calcJw >> KM4
  ./k-means HokkaidoTowns_xy.dat   10 $seed      | ./calcJw >> KM10
  ./compLearn HokkaidoCities_xy.dat 4 $seed 0.01 | ./calcJw >> CL4
  ./compLearn HokkaidoTowns_xy.dat 10 $seed 0.01 | ./calcJw >> CL10
done
paste KM4  CL4  | awk '{if($1>$2){a++};if($2>$1){b++}}END{print a,b}'
paste KM10 CL10 | awk '{if($1>$2){a++};if($2>$1){b++}}END{print a,b}'
