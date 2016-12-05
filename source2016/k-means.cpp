// k-means.cpp
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
  int numc = atoi( argv[2] );
  int seed = atoi( argv[3] );
  vector<vector<double>> vecs;
  string buf;
  ifstream ifile1( fname1 );
  while( getline(ifile1,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(atof(buf2.c_str()));
    vecs.emplace_back(vec.begin(),vec.end());
  }
  ifile1.close();
  int nvec = vecs.size();    //-- ベクトル数（データ数）Nの取得
  int ndim = vecs[0].size(); //-- ベクトルの次元数Mを先頭のベクトルから取得
  vector<int> lbls(nvec,0);  //-- ベクトルに付与するクラスタラベル用の変数
  vector<vector<double>> weight(numc);   
  for( int k = 0 ; k < numc ; k++ ) weight[k].resize(ndim,0);
  //-- クラスタラベルの初期化（randomLabelと同じ） --
  mt19937 gen(seed);
  uniform_int_distribution<int> dist(0, numc-1);
  for( int i = 0 ; i < nvec ; i++ ) lbls[i] = dist(gen);
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
