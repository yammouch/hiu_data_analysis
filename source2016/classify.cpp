// classify.cpp
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
  string fnameW = argv[2]; // 重みデータ（最後のカラムはラベルを表す）
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
  ifstream ifileW( fnameW );
  vector<vector<double>> weight;
  while( getline(ifileW,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2));
    weight.emplace_back(vec.begin(),vec.end());
  }
  ifileW.close();
  int numc = weight.size();
  vector<int> lbls(nvec,0);
  updateLabel(vecs,lbls,weight);
  //-- データにクラスラベルを付与して出力 --
  for( int i = 0 ; i < nvec ; i++ ){
    for( int m = 0 ; m < ndim ; m++ ) cout << vecs[i][m] << " ";
    cout << lbls[i] << endl;
  }
}
