// readTrpCrs.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  string fname = argv[1];
  ifstream ifile( fname );
  int nRows = 0, nCols = 0; // 行数と列数を表す変数の宣言
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
  SparseMatrix<double, RowMajor> X(nRows, nCols);   // CRS形式で宣言
  X.setFromTriplets(triplets.begin(), triplets.end());
  SparseMatrix<double, RowMajor> Xt = X.transpose();// CRS形式で宣言
  cout << "X= " << X << endl;
  cout << "X^t= " << Xt << endl;
}   
