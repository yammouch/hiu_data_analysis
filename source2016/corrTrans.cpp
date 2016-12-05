// corrTrans.cpp 相関を見るための変換（重心を原点．分散は１）
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  vector<double> vec;
  string buf;
  int nvec = 0;                     // データ数初期化
  while( getline(cin, buf) ){       // 標準入力（cin）から1行ずつ読み込む
    istringstream iss(buf);
    nvec++;                         // データ数更新
    string buf2;
    while( iss >> buf2 ) vec.emplace_back(stod(buf2)); // 配列に加える
  }
  int ndim = vec.size() / nvec;     // 次元数ndim=M算出
  MatrixXd X  = Map<MatrixXd>(&(vec[0]),ndim,nvec); // 配列を行列に変換
  cerr << "（行数，列数）= " << X.rows() << ", " << X.cols() << endl;
  MatrixXd mu = X.rowwise().mean();
  MatrixXd Sigma = 1/(double)nvec*X*X.transpose() - mu*mu.transpose();
  MatrixXd T = Sigma.diagonal().cwiseSqrt().cwiseInverse().asDiagonal();
  MatrixXd D = X.colwise() - X.rowwise().mean();
  cout << D.transpose()*T << endl;
}

