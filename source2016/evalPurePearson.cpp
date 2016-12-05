// evalPurePearson.cpp
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
  string fname1 = argv[1];
  string fname2 = argv[2];
  int nRows = 0, nCols = 0; // 行数（User）と列数（item）を表す変数の宣言
  int row, col;
  double val;
  string buf;
  typedef Triplet<double> T;
  vector<T> triplets1,triplets2;
  ifstream ifile( fname1 );
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    triplets1.emplace_back(T(row-1,col-1,val));
    if( row > nRows ) nRows = row;
    if( col > nCols ) nCols = col;
  }
  ifile.close();
  ifile.open( fname2 );
  while( getline(ifile,buf) ){
    istringstream iss(buf);
    iss >> row >> col >> val;
    triplets2.emplace_back(T(row-1,col-1,val));
    if( row > nRows ) nRows = row;
    if( col > nCols ) nCols = col;
  }
  ifile.close();
  int nItems = nCols;
  int nUsers = nRows;
  double nUsersInv = 1.0 / (double) nUsers;
  SparseMatrix<double> spX(nUsers,nItems);
  spX.setFromTriplets(triplets1.begin(), triplets1.end());
  SparseMatrix<double> spX2(nUsers,nItems);
  spX2.setFromTriplets(triplets2.begin(), triplets2.end());
  //-- 相関Pと近接モデルで使用するItemの平均評価値aveの算出
  MatrixXd Xt  = spX.transpose();
  MatrixXd mu  = Xt.rowwise().mean();
  MatrixXd Cov = nUsersInv * Xt*spX - mu*mu.transpose();
  MatrixXd A   = MatrixXd::Zero(nItems,nItems);
  for( int i = 0 ; i < nItems ; i++ )  // 評価がないitemとの相関は0に
    if( Cov(i,i) > 0 ) A(i,i) = 1.0 / sqrt(Cov(i,i)); 
  MatrixXd P   = A*Cov*A;
  VectorXd ave   = VectorXd::Zero(nItems);      
  for( int i = 0 ; i < nItems ; i++ ){
    double nNz = 0;
    for(SparseMatrix<double>::InnerIterator it(spX,i);it; ++it, nNz++)
      ave(i) += it.value();
    if( nNz != 0 ) nNz = 1.0 / nNz;
    ave(i) *= nNz;                 // 非ゼロ要素数で割り，評価の平均算出
  }
  //-- 各ユーザについて，評価履歴と相関に基づく評価値の推定と推薦の評価
  SparseMatrix<double> spXt  = spX.transpose();
  SparseMatrix<double> spX2t = spX2.transpose();
  double ndcg = 0, mse = 0, mae = 0, nEval = 0;
  for( int u = 0 ; u < nUsers ; u++ ){
    //---- 評価値の推定
    vector<double> estR(nItems,0); // 各itemについて推測した評価ratings
    vector<double> den(nItems,0);  // 各itemのrating推測に使う類似度の和
    for( SparseMatrix<double>::InnerIterator it(spXt,u); it ; ++it ){ 
      int itemU = it.row();
      int valU  = it.value();
      for( int i = 0 ; i < nItems ; i++ ){
	estR[i] += (valU - ave(itemU)) * P(itemU,i);
	den[i]  += fabs(P(itemU,i));
      }
    }
    for( int i = 0 ; i < nItems ; i++ )
      if( den[i] != 0 ) estR[i] = ave(i) + estR[i] / den[i];
      else              estR[i] = ave(i);
    //---- 推定評価値の評価
    vector<double> rate;
    multimap<double, double, greater<double>>  mapEstR2R;
    for( SparseMatrix<double>::InnerIterator it(spX2t,u); it ; ++it ){ 
      rate.emplace_back(it.value());
      mapEstR2R.insert(pair<double,double>(estR[it.row()],it.value()));
      double tmp = estR[it.row()] - it.value();
      mse += tmp * tmp;
      mae += fabs(tmp);
      nEval++;
    }
    //---- 理想的なdcg（idcg）と推定評価値estRからのdcgの算出
    sort(rate.begin(),rate.end(),greater<double>());// 評価値を降順ソート
    multimap<double, double>::iterator it2 = mapEstR2R.begin();
    double idcg = rate[0];
    double dcg = it2->second;
    it2++;
    for( int p = 1 ; p < rate.size() ; p++, it2++ ) {
      idcg += rate[p]     / log2(p+1.0);
      dcg  += it2->second / log2(p+1.0);
    }
    ndcg += dcg / idcg;
  }
  cerr << "ndcg= " << nUsersInv * ndcg  << endl;
  cerr << "rmse= " << sqrt(mse/nEval) << endl;
  cerr << "mae = " << mae/nEval << endl;
}
