// k-means++.cpp
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
  int numc = stoi( argv[2] );
  int seed = stoi( argv[3] );
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
  //-- クラスタラベルの初期化 --
  vector<double> cost(nvec, numeric_limits<double>::max());
  mt19937 gen(seed);
  uniform_int_distribution<int> idist(0, nvec-1);
  uniform_real_distribution<double> fdist(0, 1);
  int ivec = idist(gen);     //-- 最初の重みベクトルを選択
  int kW = 0;
  while( kW < numc ){
    for( int m = 0 ; m < ndim ; m++ ) weight[kW][m] = vecs[ivec][m];
    for( int i = 0 ; i < nvec ; i++ ){
      double sum = 0;        //-- 誤差計算用の変数
      for( int m = 0 ; m < ndim ; m++ )
	sum += (vecs[i][m]-weight[kW][m])*(vecs[i][m]-weight[kW][m]);
      if( cost[i] > sum ) cost[i] = sum;
    }
    double fVal = fdist(gen) * accumulate(cost.begin(),cost.end(),0.0);
    ivec = 0;
    double val = 0;
    for( int i = 0 ; i < nvec ; i++ ){
      if( fVal < val + cost[i] ){
	ivec = i;
	break;
      }
      else val += cost[i];
    }
    kW++;
  }
  updateLabel(vecs,lbls,weight);
  //-- k-meansアルゴリズムのコア（繰り返し）部分 --
  int ic = 1;
  double Jw, Eq;
  do{
    Jw = updateWeight(vecs,lbls,weight);
    Eq = updateLabel(vecs,lbls,weight);
    cerr << "ic= " << ic << ", Jw= " << Jw << ", Eq= " << Eq << endl;
    ic++;
  }while( Jw != Eq );
  //-- データにクラスタラベルを付与して出力 --
  for( int i = 0 ; i < nvec ; i++ ){
    for( int m = 0 ; m < ndim ; m++ ) cout << vecs[i][m] << " ";
    cout << lbls[i] << endl;
  }
}
