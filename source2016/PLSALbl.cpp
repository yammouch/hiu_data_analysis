// PLSALbl.cpp
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
  spCMat spX(ndim, nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  //-- ITCのための入力ベクトルspXの正規化
  for( int i = 0 ; i < nvec ; i++ ){
    double sum = 0;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      sum += it.value();
    if( sum != 0 ) sum = 1.0 / sum;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      spX.coeffRef(it.row(),i) *= sum;
  }
  string fnameC = argv[2]; // ラベルファイル．ラベルデータを用いた初期化
  ifstream ifileC( fnameC );
  vector<int> vecC;
  while( getline(ifileC,buf) ) vecC.emplace_back(stoi(buf)-1);
  ifileC.close();
  int numc = *(max_element(vecC.cbegin(),vecC.cend())) +1; // クラスタ数
  Veci lbls = Map<Veci>(&(vecC[0]),nvec); // クラスタラベル→ VectorXi
  Mat W(ndim,numc);
  double Entropy = updateWeightEntropy( spX, lbls, W );
  W += 0.0003 * Mat::Ones(ndim, numc);
  Mat H = Mat::Ones(numc, nvec);
  //-- PLSA
  double B = 0;
  spCMat spXt = spX.transpose();
  for( int i = 0 ; i < nvec ; i++ )
    for(spCMat::InnerIterator it(spX,i);it; ++it) B += it.value();
  for( int ic = 0 ; ic < 100 ; ic++ ){
    Mat W1 = Mat::Zero(ndim,numc);
    Mat H1 = Mat::Zero(numc,nvec);
    for( int m = 0 ; m < ndim ; m++ )
      for(spCMat::InnerIterator it(spXt,m);it; ++it) {
	double sumQ = W.row(m).dot(H.col(it.row()));
	for( int k = 0 ; k < numc ; k++ )
	  W1(m,k) += it.value() * W(m,k)*H(k,it.row()) / sumQ;
      }
    for( int k = 0 ; k < numc ; k++ )  W1.col(k) /= W1.col(k).sum();
    for( int i = 0 ; i < nvec ; i++ )
      for(spCMat::InnerIterator it(spX,i);it; ++it) {
	double sumQ = W.row(it.row()).dot(H.col(i));
	for( int k = 0 ; k < numc ; k++ )
	  H1(k,i) += it.value() * W(it.row(),k) * H(k,i) / sumQ;
      }
    W = W1;
    H = B / H1.sum() * H1;
    double gdkl = 0.0;
    for( int i = 0 ; i < nvec ; i++ )
      for( spCMat::InnerIterator it(spX,i) ; it ; ++it ){
	double xhat = W.row(it.row()).dot(H.col(it.col()));
	gdkl += it.value() * log(it.value()/xhat) - it.value() + xhat;
      }
    cout << gdkl << endl;
  }
}
