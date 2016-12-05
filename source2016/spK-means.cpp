// spK-means.cpp
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
  //-- 球面クラスタリングのための入力ベクトルspXの正規化 --
  for( int i = 0 ; i < nvec ; i++ ){
    double sum = 0;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      sum += it.value()*it.value();
    if( sum != 0 ) sum = 1.0 / sqrt(sum);
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      spX.coeffRef(it.row(),i) *= sum;
  }
  //-- spK-means --
  int ic = 0;
  int maxCycle = 100;
  double Cosw, Cosq;
  do{
    Cosw = updateWeightCos( spX, lbls, weight );
    Cosq = updateLabelCos( spX, lbls, weight );
    ic++;
    cerr << "ic=" <<ic << ", Cosw=" <<Cosw << ", Cosq=" <<Cosq << endl;
  }while( ic < maxCycle && Cosw != Cosq );
  cout << lbls + Veci::Ones(nvec) << endl;
}
