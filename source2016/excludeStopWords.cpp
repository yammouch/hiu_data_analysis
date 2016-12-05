// excludeStopWords.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
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
  int ndim = nRows;    // 行数は，単語種類数（特徴の次元数ndim=M）を表す
  int nvec = nCols;    // 列数は，文書数nvec=Nを表す
  SparseMatrix<double> spX(ndim,nvec);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  SparseMatrix<double> spXt = spX.transpose();
  //-- ストップワードの単語id（idは0始まり）集合の作成
  string fnameVoca = argv[2];
  string fnameStop = argv[3];
  set<int> stopids;    // ストップワードの単語id（idは0始まり）集合
  map<string, int> w2id; // 単語（string）から単語idへのマップ
  ifile.open(fnameVoca);
  int id = 0;
  while( ifile >> buf ) w2id.insert(pair<string, int>(buf,id++));
  ifile.close();
  ifile.open(fnameStop);  
  while( ifile >> buf )
    if( w2id.find(buf) != w2id.end() ) stopids.insert(w2id[buf]);
  id = 0;        // 単語id用．（ストップワードを除きながらふりなおします）
  for( int m = 0 ; m < ndim ; m++ ){        // すべての単語について
    if( stopids.find(m) == stopids.end() ){ // ストップワードでなければ
      for(SparseMatrix<double>::InnerIterator it(spXt,m);it; ++it)
	cout << id+1 << " " << it.row()+1 << " " << it.value() << endl;
      id++;
    }
  }
}
