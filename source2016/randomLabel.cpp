// randomLabel.cpp
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
  mt19937 gen(seed);
  uniform_int_distribution<int> dist(0, numc-1);
  for( int i = 0 ; i < nvec ; i++ ){
    for( int m = 0 ; m < ndim ; m++ ) cout << vecs[i][m] << " ";
    cout << dist(gen) << endl;
  }
}
