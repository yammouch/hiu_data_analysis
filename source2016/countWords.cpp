// countWords.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <random>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "commonDoc.h"
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  string fname = argv[1];
  ifstream ifile( fname );
  int nRows = 0, nCols = 0; // 行数（ndim）と列数（文書数）を表す変数の宣言
  int row, col;
  double val;
  string buf;
  typedef Triplet<double> T;
  vector<T> triplets;
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    triplets.emplace_back(T(row-1,col-1,val));
    if( row > nRows ) nRows = row;
    if( col > nCols ) nCols = col;
  }
  ifile.close();
  int ndim = nRows;
  int nvec = nCols;
  spCMat spX(ndim,nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  //-- クラスタラベル読み込み
  string fnameC = argv[2]; 
  ifstream ifileC( fnameC );
  vector<int> vecC;
  while( getline(ifileC,buf) ) vecC.emplace_back(stoi(buf)-1);
  map<int, int> idcount;
  map<int, string> id2w;
  //-- 単語List読み込み
  string fnameVoca = argv[3];
  ifile.open(fnameVoca);
  int id = 0;
  while( ifile >> buf ) id2w.insert(pair<int, string>(id++,buf));
  ifile.close();
  int num = stoi( argv[4] ); //-- 注目するクラスタ（1始まり）
  //-- 特定クラスタの単語数のカウント
  for( int i = 0 ; i < nvec ; i++ )
    if( vecC[i] == num-1 )
      for(spCMat::InnerIterator it(spX,i);it; ++it)
	idcount[it.row()] += it.value();
  for(map<int,int>::iterator it=idcount.begin();it!=idcount.end();it++)
    cout << id2w[it->first] << " " << it->second << endl;
}
