// sdK-means.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <random>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "commonDoc.h"
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  string fname = argv[1];
  ifstream ifile( fname );
  int nRows = 0, nCols = 0; // 行数（ndim）と列数（文書数）を表す変数の宣言
  int row, col;
  double val;
  string buf;
  typedef Triplet<double> T;
  vector<T> triplets;
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    triplets.emplace_back(T(row-1,col-1,val));
    if( row > nRows ) nRows = row;
    if( col > nCols ) nCols = col;
  }
  ifile.close();
  int ndim = nRows;
  int nvec = nCols;
  spCMat spX(ndim,nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  Veci lbls(nvec);
  int numc = stoi( argv[2] );
  int seed = stoi( argv[3] );
  mt19937 gen(seed);
  uniform_int_distribution<int> dist(0,numc-1);
  for( int i = 0 ; i < nvec ; i++ ) lbls(i) = dist(gen);
  Mat weight(ndim,numc);
  //-- ITCのための入力ベクトルspXの正規化 --
  for( int i = 0 ; i < nvec ; i++ ){
    double sum = 0;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      sum += it.value();
    if( sum != 0 ) sum = 1.0 / sum;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      spX.coeffRef(it.row(),i) *= sum;
  }
  //-- sd-Kmeans --
  double a = stod( argv[4] );  // alpha
  int maxCycle = 100;
  double curEntropy=updateWeightEntropy(spX,lbls,weight), prevEntropy;
  for( int j = 0 ; j < 5 ; j++ ){
    int ic = 0;
    do{
      prevEntropy = curEntropy;
      updateLabelEntropy( spX, lbls, weight, a );
      curEntropy = updateWeightEntropy( spX, lbls, weight );
      ic++;
      cerr << "ic= " << ic << ", Entropy = " << curEntropy << endl;
    }while(ic < maxCycle && (prevEntropy-curEntropy)/curEntropy > 1e-8);
    a = 1.0 - (1.0 - a) * 0.1;
  }
  cout << lbls + Veci::Ones(nvec) << endl;
}
