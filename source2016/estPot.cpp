// estPot.cpp
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
  string fnamePunch = argv[3];       // フルーツポンチファイル
  string buf;
  vector<vector<double>> RatioVecs;  // 果物が出現する条件付き確率を格納
  ifstream ifileRatio( fnameRatio );
  while( getline(ifileRatio,buf) ){
    istringstream iss(buf);
    vector<double> vec;
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2));
    RatioVecs.emplace_back(vec.begin(),vec.end());
  }
  ifileRatio.close();
  int ndim = RatioVecs.size();       // 特徴数M(ndim)＝果物の種類数
  int numc = RatioVecs[0].size();    // クラス数K(numc)＝壺の数
  vector<double> Prior(numc,0);      // 事前確率（クラス数（壺数）ある）
  ifstream ifilePrior( fnamePrior );
  for( int k = 0 ; k < numc ; k++ ) ifilePrior >> Prior[k];
  ifilePrior.close();
  vector<vector<double>> Punch;      // フルーツポンチの情報を格納
  vector<int> Cnum;                  // もし，クラス（壺）情報があれば格納
  ifstream ifilePunch( fnamePunch );
  while( getline(ifilePunch,buf) ){
    istringstream iss(buf);
    vector<string> vbuf;
    vector<double> vec;
    string buf2;
    //-- カンマ区切りで，前半（ボウル情報）と後半（壺情報）を分離 
    while( getline(iss, buf2, ',') )  vbuf.emplace_back(buf2);        
    if(vbuf.size()==2) Cnum.emplace_back(stoi(vbuf[1])); //真の壺情報格納
    else               Cnum.emplace_back(-1); //壺情報がないときは-1とする
    istringstream iss2(vbuf[0]);
    //-- カンマ区切りの前半（ボウル情報）を空白区切りで，切り出し
    while( iss2 >> buf2 ) vec.emplace_back(stod(buf2));
    Punch.emplace_back(vec.begin(),vec.end());
  }
  ifilePunch.close();
  int nvec = Punch.size();                 // フルーツポンチのボウル数N
  //-- 条件付き確率と事前確率を自然対数へ変換（対数事後確率計算のため）
  for( int m = 0 ; m < ndim ; m++ )
    for( int k = 0 ; k < numc ; k++ )
      RatioVecs[m][k] = log(RatioVecs[m][k]);
  for( int k = 0 ; k < numc ; k++ )
    Prior[k] = log(Prior[k]);
  //-- 推定
  for( int i = 0 ; i < nvec ; i++ ){
    vector<double> logPosteriori(numc,0);  // 事後確率を0で初期化
    for(int k=0; k<numc; k++) logPosteriori[k] = Prior[k];
    int num = Punch[i].size();             // 果物の数
    for( int j = 0 ; j < num ; j++ ){
      int Wm = Punch[i][j];                // 果物の種類（mの値）
      for(int k=0; k<numc; k++) logPosteriori[k] += RatioVecs[Wm][k];
    }
    int maxk = -1;
    double max_val = -numeric_limits<double>::max();
    for( int k = 0 ; k < numc ; k++ )
      if( logPosteriori[k] > max_val ){
	maxk = k;
	max_val = logPosteriori[k];
      }
    for( int k = 0 ; k < numc-1 ; k++ ) cout << logPosteriori[k] << " ";
    cout << logPosteriori[numc-1];
    cout << "," << Cnum[i] << "," << maxk << endl;
  }
}
