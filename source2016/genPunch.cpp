// genPunch.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
using namespace std;
int main(int argc, char* argv[]){
  string fnameRatio = argv[1];
  string fnamePrior = argv[2];
  int nvec          = atoi(argv[3]); // ボウル数＝ベクトル数N
  int nFruits       = atoi(argv[4]); // １つのボウルのために生成する果物数
  int seed          = atoi(argv[5]); // 乱数の種
  string buf;
  vector<vector<double>> RatioVecs;  // 果物が出現する条件付き確率を格納
  ifstream ifileRatio( fnameRatio );
  while( getline(ifileRatio,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(atof(buf2.c_str()));
    RatioVecs.emplace_back(vec.begin(),vec.end());
  }
  ifileRatio.close();
  int ndim = RatioVecs.size();    // 特徴数M(ndim)＝果物の種類数
  int numc = RatioVecs[0].size(); // クラス数K(numc)＝壺の数
  vector<double> Prior(numc,0);   // 事前確率（クラス数（壺数）ある）
  ifstream ifilePrior( fnamePrior );
  for( int k = 0 ; k < numc ; k++ ) ifilePrior >> Prior[k];
  ifilePrior.close();

  mt19937 gen(seed);
  uniform_real_distribution<double> dist(0, 1); // [0,1)の浮動小数点乱数
  for( int i = 0 ; i < nvec ; i++ ){
    //-- クラス（壺）の選択
    double rValue = dist(gen);                  // [0,1)の乱数生成
    int Ck = 0;
    double val = 0;
    for( int k = 0 ; k < numc ; k++ ){
      if( rValue < val + Prior[k] ){
	Ck = k;
	break;
      }
      else val += Prior[k];
    }
    //-- 特徴（＝果物）の選択と出力
    for( int j = 0 ; j < nFruits ; j++ ){
      double rValue = dist(gen);                // [0,1)の乱数生成
      int Wm = 0;
      double val = 0;
      for( int m = 0 ; m < ndim ; m++ ){
        if( rValue < val + RatioVecs[m][Ck] ){
	  Wm = m;
	  break;
	}
        else val += RatioVecs[m][Ck];
      }
      if( j != nFruits - 1 ) cout << Wm << " ";
      else                   cout << Wm;
    }
    cout << "," << Ck << endl;
  }
}
