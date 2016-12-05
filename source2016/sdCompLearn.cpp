// sdCompLearn.cpp
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
  //-- sdCL競合学習 --
  double  gma = stod( argv[4] );
  double    a = 1.0 - gma; //alpha
  double   a2 = 1.0 - a;
  double gma2 = 1.0 - gma;
  updateWeightEntropy( spX, lbls, weight );
  uniform_int_distribution<int> dist2(0, nvec-1); // 入力ベクトル選択用
  int maxCycle = 1000000;                         // 繰り返し回数設定
  for( int ic = 0 ; ic < maxCycle ; ic++ ){
    if( ic % 100000 == 0 ) cerr << "ic=" << ic << endl;
    int iVec = dist2(gen);                        // 入力ベクトル選択
    int kWin = -1;                                // 勝者ベクトル添字
    double minE = std::numeric_limits<double>::max(); //最小距離初期化
    for( int k = 0 ; k < numc ; k++ ){            // 勝者の探索
      double sKL = 0;      //-- skew(KL)Div.計算用の変数
      for(spCMat::InnerIterator it(spX,iVec);it; ++it){ 
	double tmp = a*weight(it.row(),k) + a2*it.value();
	sKL -= it.value() * log2(tmp);
      }
      if( sKL < minE ){    //-- もし「最小距離」の候補より小さいなら
	minE = sKL;        //-- 「最小距離」の候補の更新
	kWin = k;          //-- クラスタラベル候補の更新
      }
    }
    weight.col(kWin) *= gma2;
    for(spCMat::InnerIterator it(spX,iVec);it; ++it) 
      weight(it.row(),kWin) += gma * it.value();
  }
  //-- sd-Kmeans
  a = stod( argv[5] );
  maxCycle = 100;
  double curEntropy=100, prevEntropy;
  for( int j = 0 ; j < 5 ; j++ ){
    int ic = 0;
    do{
      prevEntropy = curEntropy;
      updateLabelEntropy( spX, lbls, weight, a );
      curEntropy  = updateWeightEntropy( spX, lbls, weight );
      ic++;
      cerr << "ic= " << ic << ", Entropy = " << curEntropy << endl;
    }while(ic < maxCycle && (prevEntropy-curEntropy)/curEntropy > 1e-8);
    a = 1.0 - (1.0 - a) * 0.1;
  }
  cout << lbls + Veci::Ones(nvec) << endl;
}
