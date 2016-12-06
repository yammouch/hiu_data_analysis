// modPerceptron.cpp
#include "common.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <algorithm>
using namespace std;
int main(int argc, char* argv[]){
  string fname1 = argv[1];
  int      seed = atoi( argv[2] );
  double    gma = atof( argv[3] );
  vector<vector<double>> vecs;
  vector<int> lbls;
  string buf;
  ifstream ifile1( fname1 );
  while( getline(ifile1,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(atof(buf2.c_str()));
    vecs.emplace_back(vec.begin(),prev(vec.end()));
    lbls.emplace_back((int)vec.back());
  }
  ifile1.close();
  int nvec = vecs.size();    //-- ベクトル数（データ数）Nの取得
  int ndim = vecs[0].size(); //-- ベクトルの次元数Mを先頭のベクトルから取得
  int numc = *(max_element(lbls.cbegin(), lbls.cend())) +1; // クラス数K
  vector<vector<double>> weight(numc);
  for( int k = 0 ; k < numc ; k++ ) weight[k].resize(ndim,0);
  mt19937 gen(seed);
  uniform_int_distribution<int> dist(0, nvec-1);
  for( int k = 0 ; k < numc ; k++ ){
    int i = dist(gen);
    for( int m = 0 ; m < ndim ; m++ ) weight[k][m] = vecs[i][m];
  }
  //--
  int maxIter = 1000;        //-- 学習パターン全体の処理の繰り返し回数の上限
  int nLearn;                //-- 学習が必要であった回数
  for( int ic = 0 ; ic < maxIter ; ic++ ){
    nLearn = 0;
    for( int i = 0 ; i < nvec ; i++ ){
      int cClass = -1;                            //最近傍代表ベクトル添字
      double minE = numeric_limits<double>::max();//最小誤差初期化
      for( int k = 0 ; k < numc ; k++ ){          //最近傍の探索
        double sum = 0;
        for( int m = 0 ; m < ndim ; m++ ){
          double tmp = vecs[i][m] - weight[k][m];
          sum += tmp * tmp;
        }
        if( sum < minE ){
          minE = sum;
          cClass = k;
        }
      }
      int tClass = lbls[i];
      if( tClass != cClass ){
        nLearn++;
        for( int m = 0 ; m < ndim ; m++ ){
          weight[tClass][m]=(1.0-gma)*weight[tClass][m] +gma*vecs[i][m];
          weight[cClass][m]=(1.0+gma)*weight[cClass][m] -gma*vecs[i][m];
        }
      }
    }
    cerr << "ic= " << ic << ", nLearn= " << nLearn << endl;
    if( nLearn == 0 ) break;
  }
  //-- 代表ベクトルの出力 --
  for( int k = 0 ; k < numc ; k++ ){
    for( int m = 0 ; m < ndim-1 ; m++ ) cout << weight[k][m] << " ";
    cout << weight[k][ndim-1] << endl;
  }
}
