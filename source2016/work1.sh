for seed in `seq 0 29`
do
  ./sdK-means 20ng.dat 20 $seed 0.99 > 20ngResult.dat
  ./evalClustering 20ng.label 20ngResult.dat 20ng.dat >> sdKresult1.dat
done
