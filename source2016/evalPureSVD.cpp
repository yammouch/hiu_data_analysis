// evalPureSVD.cpp
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
  //-- SVDの実行と評価値の推定（＝推定行列estRMの計算）
  int k = stoi(argv[3]);
  MatrixXd X = spX;
  JacobiSVD<MatrixXd> svd(X, ComputeThinU | ComputeThinV);
  MatrixXd Vk = svd.matrixV().block(0,0,nItems,k);
  MatrixXd Uk = svd.matrixU().block(0,0,nUsers,k);
  VectorXd S  = svd.singularValues();
  MatrixXd Sk = MatrixXd::Zero(k,k);
  for( int j = 0 ; j < k ; j++ ) Sk(j,j) = S(j);
  MatrixXd estRM = Uk * Sk * Vk.transpose();
  //-- 各ユーザについて，評価値の推定と推薦の評価
  SparseMatrix<double> spXt  = spX.transpose();
  SparseMatrix<double> spX2t = spX2.transpose();
  double ndcg = 0, mse = 0, mae = 0, nEval = 0;
  for( int u = 0 ; u < nUsers ; u++ ){
    //---- 評価値の推定
    vector<double> estR(nItems,0); // 各itemについて推測した評価ratings
    for( int i = 0 ; i < nItems ; i++ ) estR[i] = estRM(u,i);
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
