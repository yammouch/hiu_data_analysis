// compLearn.cpp
#include "common.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
using namespace std;
int main(int argc, char* argv[]){
  string fname1 = argv[1];
  int      numc = stoi( argv[2] );
  int      seed = stoi( argv[3] );
  double    gma = stod( argv[4] );
  vector<vector<double>> vecs;
  string buf;
  ifstream ifile1( fname1 );
  while( getline(ifile1,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2));
    vecs.emplace_back(vec.begin(),vec.end());
  }
  ifile1.close();
  int nvec = vecs.size();    //-- ベクトル数（データ数）Nの取得
  int ndim = vecs[0].size(); //-- ベクトルの次元数Mを先頭のベクトルから取得
  vector<int> lbls(nvec,0);  //-- ベクトルに付与するクラスタラベル用の変数
  vector<vector<double>> weight(numc);
  for( int k = 0 ; k < numc ; k++ ) weight[k].resize(ndim,0);
  //-- 重み（＝代表）ベクトルの初期化（ランダムラベル＋重みベクトル更新）
  mt19937 gen(seed);
  uniform_int_distribution<int> dist(0, numc-1);      // ランダムラベル用
  for( int i = 0 ; i < nvec ; i++ ) lbls[i] = dist(gen); //ランダムラベル
  updateWeight(vecs,lbls,weight);                 // 重みベクトル一括更新
  //-- 競合学習 --
  uniform_int_distribution<int> dist2(0, nvec-1); // 入力ベクトル選択用
  int maxCycle = 1000000;                         // 繰り返し回数設定
  for( int ic = 0 ; ic < maxCycle ; ic++ ){
    int iVec = dist2(gen);                        // 入力ベクトル選択
    int kWin = -1;                                // 勝者ベクトル添字
    double minE = numeric_limits<double>::max();  // 最小誤差初期化
    for( int k = 0 ; k < numc ; k++ ){            // 勝者の探索
      double sum = 0;
      for( int m = 0 ; m < ndim ; m++ ){
        double tmp = vecs[iVec][m] - weight[k][m];
        sum += tmp * tmp;
      }
      if( sum < minE ){
        minE = sum;
        kWin = k;
      }
    }
    for( int m = 0 ; m < ndim ; m++ )             // 勝者の更新（学習）
      weight[kWin][m] = (1.0 - gma)*weight[kWin][m] + gma*vecs[iVec][m];
  }
  //-- クラスタラベルの算出とクラスタ内平方和Jwの算出 --
  double Eq = updateLabel(vecs,lbls,weight); // クラスタラベルの算出
  double Jw = updateWeight(vecs,lbls,weight);// Jwの算出
  cerr << "Eq= " << Eq << ", Jw= " << Jw << endl;
  //-- データにクラスタラベルを付与して出力 --
  for( int i = 0 ; i < nvec ; i++ ){
    for( int m = 0 ; m < ndim ; m++ ) cout << vecs[i][m] << " ";
    cout << lbls[i] << endl;
  }
}
