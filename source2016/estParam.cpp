// estParam.cpp
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
  string fnamePunch = argv[3];   // フルーツポンチファイル
  int numc = atoi(argv[4]);      // クラス（＝壺）数
  int ndim = atoi(argv[5]);      // 特徴（＝果物）数
  string buf;
  vector<vector<double>> Punch;  // フルーツポンチの情報を格納
  vector<int> Cnum;              // もし，クラス（壺）情報があれば格納
  ifstream ifilePunch( fnamePunch );
  while( getline(ifilePunch,buf) ){
    istringstream iss(buf);
    vector<string> vbuf;
    vector<double> vec;
    string buf2;
    //-- カンマ区切りで，前半（ボウル情報）と後半（壺情報）を分離 
    while( getline(iss, buf2, ',') )  vbuf.emplace_back(buf2);        
    if(vbuf.size()==2) Cnum.emplace_back(atoi(vbuf[1].c_str())); //真の壺情報格納
    else               Cnum.emplace_back(-1); //壺情報が無いときは-1とする
    istringstream iss2(vbuf[0]);
    //-- カンマ区切りの前半（ボウル情報）を空白区切りで，切り出し
    while( iss2 >> buf2 ) vec.emplace_back(atof(buf2.c_str()));
    Punch.emplace_back(vec.begin(),vec.end());
  }
  ifilePunch.close();
  int nvec = Punch.size(); // フルーツポンチの数（ボウルの数）
  //-- 事前確率の推定（頻度／総頻度）．0頻度を避けるため"1"を全クラスに与える
  vector<double> Prior(numc,1); // 事前確率（まず壺の頻度を格納，1で初期化）
  for( int i = 0 ; i < nvec ; i++ )
    Prior[Cnum[i]]++;
  double sum = 0;
  for( int k = 0 ; k < numc ; k++ )  sum += Prior[k];  // 総頻度計算
  for( int k = 0 ; k < numc ; k++ )  Prior[k] /= sum;  // 頻度／総頻度
  //-- フルーツ（果物）の割合（壺ごとに果物が出現する条件付き確率）の推定
  vector<vector<double>> RatioVecs(ndim); // 果物の頻度／総頻度
  for( int m=0 ; m<ndim ; m++ ) RatioVecs[m].resize(numc,1); //1で初期化
  for( int i = 0 ; i < nvec ; i++ ){
    int k = Cnum[i];       // クラス（壺）情報
    for( int j = 0 ; j < Punch[i].size() ; j++ ){
      int m = Punch[i][j]; // 特徴（果物）情報
      RatioVecs[m][k]++;   // 頻度をプラス1
    }
  }
  for( int k = 0 ; k < numc ; k++ ){
    double sum = 0;
    for( int m=0 ; m<ndim ; m++ ) sum += RatioVecs[m][k];
    for( int m=0 ; m<ndim ; m++ ) RatioVecs[m][k] /= sum; // ／総頻度
  }
  //--推定したパラメータ（モデルの）を出力する
  ofstream ofileRatio( fnameRatio );
  for( int m = 0 ; m < ndim ; m++ ){
    for(int k=0; k < numc-1; k++) ofileRatio << RatioVecs[m][k] << " ";
    ofileRatio << RatioVecs[m][numc-1] << endl;
  }
  ofileRatio.close();
  ofstream ofilePrior( fnamePrior );
  for( int k = 0 ; k < numc - 1 ; k++ ) ofilePrior << Prior[k] << " ";
  ofilePrior << Prior[numc-1] << endl;
  ofilePrior.close();
}
