// classifyDoc.cpp
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
  string fnameLogRatio = argv[1];  // モデルパラメータファイルの読み込み
  string buf;
  vector<double> vec;
  vector<vector<double>> logRatio;  
  ifstream ifileLogRatio( fnameLogRatio );
  while( getline(ifileLogRatio, buf) ){       
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2));
    logRatio.emplace_back(vec.begin(),vec.end());
  }
  ifileLogRatio.close();
  int ndim = logRatio.size();      // 次元数ndim=M（単語種類数）
  int numc = logRatio[0].size();   // クラス数numc=K
  //-- 文書の読み込み．分類
  string fname = argv[2];
  int nRows = ndim;
  ifstream ifile( fname );
  int row, col;
  int nCols = 0;                   // 列数（文書数）を表す変数の宣言
  double val;
  typedef Triplet<double> T;
  vector<T> triplets;
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    if( row <= nRows ){
      triplets.emplace_back(T(row-1,col-1,val));
      if( col > nCols ) nCols = col;
    }
  }
  ifile.close();
  int nvec = nCols;
  spCMat spX(ndim,nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  Veci lbls = Veci::Zero(nvec);
  for( int i = 0 ; i < nvec ; i++ ){
    int label = -1;             // クラスタラベルの仮設定（以下で更新）
    double maxL = -numeric_limits<double>::max();      // 初期値設定
    for( int k = 0 ; k < numc ; k++ ){ // 尤度最大となるクラスkの探索
      double logLike = 0;
      for(spCMat::InnerIterator it(spX,i);it; ++it) 
	logLike += it.value() * logRatio[it.row()][k];
      if( logLike > maxL ){            // 尤度最大となるクラスkの更新
	maxL  = logLike;
	label = k;
      }
    }
    lbls(i) = label;
  }
  cout << lbls + Veci::Ones(nvec) << endl;
}
