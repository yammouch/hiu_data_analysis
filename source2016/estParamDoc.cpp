// estParamDoc.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "commonDoc.h"
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  string fname = argv[1];
  int nRows = stoi( argv[3] );          // 行数（ndim,単語種類数）
  ifstream ifile( fname );
  int row, col;
  int nCols = 0;                        // 列数（文書数）を表す変数の宣言
  string buf;
  double val;
  typedef Triplet<double> T;
  vector<T> triplets;
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    triplets.emplace_back(T(row-1,col-1,val));
    if( col > nCols ) nCols = col;
  }
  ifile.close();
  int ndim = nRows;
  int nvec = nCols;
  spCMat spX(ndim,nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  //-- クラスラベルデータの読み込み．単語出現数（割合）の算出
  string fnameC = argv[2];              //（クラスラベル）ファイル
  ifstream ifileC( fnameC );
  vector<int> vecC;
  while( getline(ifileC,buf) ) vecC.emplace_back(stoi(buf)-1);
  ifileC.close();
  int numc = *(max_element(vecC.cbegin(),vecC.cend())) +1; // クラス数
  Mat Ratio = Mat::Zero(ndim,numc);
  int numR = 10;                        // この値未満の頻度rを補正する
  for( int i = 0 ; i < nvec ; i++ )
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      Ratio(it.row(),vecC[i]) += it.value();
  for( int k = 0 ; k < numc ; k++ ){
    Vec Nr = Vec::Zero(numR+1);         // ディスカウンティングbegin
    for( int m = 0 ; m < ndim ; m++ ){
      int r = Ratio(m,k);
      if( r <= numR ) Nr(r)++;          // 出現回数rの単語の種類数算出
    }
    for( int m = 0 ; m < ndim ; m++ ){
      int r = Ratio(m,k);
      if( r <= numR -1 ) Ratio(m,k) = (r+1)*Nr(r+1)/Nr(r);
    }                                   // ディスカウンティングend
    double sum = Ratio.col(k).sum();    // 総単語出現数（補正あり）
    if( sum != 0 ) Ratio.col(k) /= sum; // 割合（確率）値へ変換
    for( int m = 0 ; m < ndim ; m++ ) 
      Ratio(m,k) = log(Ratio(m,k));     // 対数尤度へ変換
  }
  cout << Ratio << endl;
}
