// calcP.cpp 相関行列の算出
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
  MatrixXd Cov = 1/(double)nvec*X*X.transpose() - mu*mu.transpose();
  MatrixXd T = Cov.diagonal().cwiseSqrt().cwiseInverse().asDiagonal();
  cout << T*Cov*T << endl;
}

