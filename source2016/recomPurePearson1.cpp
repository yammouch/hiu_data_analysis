// recomPurePearson1.cpp
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
  int nRows = 0, nCols = 0; // 行数（User）と列数（item）を表す変数の宣言
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
  int nItems = nCols;
  int nUsers = nRows;
  double nUsersInv = 1.0 / (double) nUsers;
  SparseMatrix<double> spX(nUsers,nItems);
  spX.setFromTriplets(triplets.begin(), triplets.end());
  MatrixXd Xt  = spX.transpose();
  MatrixXd mu  = Xt.rowwise().mean();
  MatrixXd Cov = nUsersInv * Xt*spX - mu*mu.transpose();
  MatrixXd A   = MatrixXd::Zero(nItems,nItems);
  for( int i = 0 ; i < nItems ; i++ )  // 評価がないitemとの相関は0に
    if( Cov(i,i) > 0 ) A(i,i) = 1.0 / sqrt(Cov(i,i)); 
  MatrixXd P   = A*Cov*A;              // 相関行列算出．-- ここまで共通 --
  //-- 各アイテムに対し，相関が高いアイテムを相関係数の値の降順に出力
  for( int i = 0 ; i < nItems ; i++ ){
    cout << i+1;
    multimap<double, int, greater<double> > mapP2itemId;// 降順整列用
    for( int j = 0 ; j < nItems ; j++ )
      if( j != i )
	mapP2itemId.insert(pair<double, int>(P(i,j),j));// 相関係数で整列
    multimap<double, int>::iterator it = mapP2itemId.begin();
    for( int j = 0 ; j < 3 ; it++, j++ ) cout << " " << it->second+1;
    cout << endl;
  }
}
