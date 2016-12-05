// KLnmfLbl.cpp
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
  //-- NMF
  for( int ic = 0 ; ic < 100 ; ic++ ){
    std::vector<T> tripletList;     //----- Wの更新 -----
    tripletList.reserve(spX.nonZeros());
    for( int i = 0 ; i < nvec ; i++ ){
      for( spCMat::InnerIterator it(spX,i) ; it ; ++it ){
	double val = it.value()/(W.row(it.row()).dot(H.col(it.col())));
	tripletList.emplace_back(T(it.col(),it.row(),val));
      }
    }
    spCMat XRt(nvec, ndim);
    XRt.setFromTriplets(tripletList.begin(), tripletList.end());
    Mat Wnum = H * XRt;
    Mat Wden = H.rowwise().sum();
    for( int m = 0 ; m < ndim ; m++ )
      W.row(m).array() *=  Wnum.col(m).array() / Wden.array();
    Vec Sum = W.colwise().sum();
    for( int k = 0 ; k < numc ; k++ )  W.col(k) /= Sum(k);
    Mat Hnum = XRt * W;             //----- Htの更新 -----
    Mat Hden = W.colwise().sum();
    for( int i = 0 ; i < nvec ; i++ )
      H.col(i).array() *= Hnum.row(i).array() / Hden.array();
    double gdkl = 0.0;
    for( int i = 0 ; i < nvec ; i++ )
      for( spCMat::InnerIterator it(spX,i) ; it ; ++it ){
	double xhat = W.row(it.row()).dot(H.col(it.col()));
	gdkl += it.value() * log(it.value()/xhat) - it.value() + xhat;
      }
    cout << gdkl << endl;
  }
}
