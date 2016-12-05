// recomPurePearson2.cpp
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
  //-- 各ユーザに対し，評価履歴と相関に基いて推薦リストを作成
  VectorXd ave   = VectorXd::Zero(nItems);      
  for( int i = 0 ; i < nItems ; i++ ){
    double nNz = 0;
    for(SparseMatrix<double>::InnerIterator it(spX,i);it; ++it, nNz++)
      ave(i) += it.value();
    if( nNz != 0 ) nNz = 1.0 / nNz;
    ave(i) *= nNz;                 // 非ゼロ要素数で割り，評価の平均算出
  }
  SparseMatrix<double> spXt = spX.transpose();
  for( int u = 0 ; u < nUsers ; u++ ){
    vector<double> estR(nItems,0); // 各itemについて推測したratings
    vector<double> den(nItems,0);  // 各itemのrating推測に使う類似度の和
    set<int> Iu;
    for( SparseMatrix<double>::InnerIterator it(spXt,u); it ; ++it ){ 
      int itemU = it.row();
      Iu.insert(itemU);
      int valU  = it.value();
      for( int i = 0 ; i < nItems ; i++ ){
	estR[i] += (valU - ave(itemU)) * P(itemU,i);
	den[i]  += fabs(P(itemU,i));
      }
    }
    for( int i = 0 ; i < nItems ; i++ )
      if( den[i] != 0 ) estR[i] = ave(i) + estR[i] / den[i];
      else              estR[i] = ave(i);
    multimap<double, int, greater<double>> mapEstR2itemId;
    for( int i = 0 ; i < nItems ; i++ )
      if( Iu.find(i) == Iu.end() )
	mapEstR2itemId.insert(pair<double, int>(estR[i],i));
    multimap<double,int>::iterator it = mapEstR2itemId.begin();
    for( int j = 0 ; j < 5 ; j++, it++ )
      cout << " " << it->second+1;
    cout << endl;
  }
}
