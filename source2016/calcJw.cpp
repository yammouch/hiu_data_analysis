// calcJw.cpp
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;
int main(int argc, char* argv[]){
  vector<vector<double>> vecs;
  vector<int> lbls;
  string buf;
  while( getline(cin,buf) ){ //-- 標準入力（cin）から1行ずつ読み込む
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2));
    vecs.emplace_back(vec.begin(),prev(vec.end()));
    lbls.emplace_back((int)vec.back());
  }
  int nvec = vecs.size();    //-- ベクトル数（データ数）Nの取得
  int ndim = vecs[0].size(); //-- ベクトルの次元数Mを先頭のベクトルから取得
  int numc = *(max_element(lbls.cbegin(),lbls.cend())) +1; //クラスタ数K
  //-- クラスタの重心μの計算 --
  vector<vector<double>> mu(numc); // クラスタ数numc=Kの平均ベクトルを宣言   
  vector<double> n(numc,0);        // クラスタに含まれるベクトル数の宣言
  for( int i = 0 ; i < numc ; i++ ) mu[i].resize(ndim,0); //次元数M確保
  for( int i = 0 ; i < nvec ; i++ ){ // すべての入力ベクトルnvec=Nについて
    int label = lbls[i];        // i番目の入力ベクトルのクラスタラベル取得
    n[label]++;                 // クラスタに含まれるベクトル数カウントアップ
    for( int m = 0 ; m < ndim ; m++ ) mu[label][m] += vecs[i][m];
  }
  for( int k = 0 ; k < numc ; k++ )
    if( n[k] != 0 ) 
      for( int m = 0 ; m < ndim ; m++ ) mu[k][m] *= 1.0 / n[k];
  //-- クラスタ内平方和Jw計算 --
  double Jw = 0;            
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls[i];
    for( int m = 0 ; m < ndim ; m++ ){
      double tmp = vecs[i][m] - mu[label][m];
      Jw += tmp * tmp;
    }
  }
  cout << Jw << endl;
}
