// discrimTrans.cpp
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  vector<double> vec;
  string buf;
  int nvec = 0;               // データ数初期化
  vector<string> lbls;
  set<string> lblSet;
  while( getline(cin, buf) ){ // 標準入力（cin）から1行ずつ読み込む
    istringstream iss(buf);
    nvec++;                   // データ数更新
    string buf2;
    vector<string> vbuf;
    while( iss >> buf2 ) vbuf.emplace_back(buf2);
    for(int m=0; m < vbuf.size()-1;m++) vec.emplace_back(stod(vbuf[m]));
    lbls.emplace_back(vbuf.back());
    lblSet.insert(vbuf.back());
  }
  int ndim = vec.size() / nvec;              // 次元数ndim=M算出
  MatrixXd X = Map<MatrixXd>(&(vec[0]),ndim,nvec);
  MatrixXd mu = X.rowwise().mean();
  int numc = lblSet.size();                  // クラス数取得
  set<string>::iterator it = lblSet.begin(); // クラスラベルのイテレータ
  MatrixXd SW = MatrixXd::Zero(ndim,ndim);   // クラス内変動行列の初期化
  MatrixXd SB = MatrixXd::Zero(ndim,ndim);   // クラス内変動行列の初期化
  for( int k = 0 ; k < numc ; k++ ){         // 各クラスの変動行列算出
    vector<double> vec1;
    int num = 0;
    int j = 0;
    for( int i = 0 ; i < nvec ; i++ ){
      if( lbls[i] == *it ){ 
	for( int m = 0 ; m < ndim ; m++ ) vec1.emplace_back(vec[j++]);
	num++;
      }
      else j += ndim;
    }
    MatrixXd Xk  = Map<MatrixXd>(&(vec1[0]),ndim,num); // クラスkのX
    MatrixXd muk = Xk.rowwise().mean();                // クラスの重心
    SW += Xk*Xk.transpose() - (double)num*muk*muk.transpose();
    SB += (double)num*muk*muk.transpose();
    it++;
  }
  SB -= (double)nvec*mu*mu.transpose();
  SelfAdjointEigenSolver<MatrixXd> es1(SW);     // SWの固有値分解
  MatrixXd Ev=es1.eigenvectors();               // 固有ベクトル行列
  MatrixXd T1=es1.eigenvalues().cwiseSqrt().cwiseInverse().asDiagonal();
  MatrixXd A1=Ev*T1;                            // SWの白色化変換行列A1
  MatrixXd A1SBA1 = A1.transpose() * SB * A1;   // A1^t*SB*A1の算出
  SelfAdjointEigenSolver<MatrixXd> es2(A1SBA1); // A1^t*SB*A1の固有値分解
  MatrixXd A2 = es2.eigenvectors();             // 固有ベクトル行列A2
  MatrixXd A = (A1*A2).block(0,ndim-numc+1,ndim,numc-1);//K-1次元の抽出
  cerr << "SW' = " << endl << A.transpose()*SW*A << endl;
  cerr << "SB' = " << endl << A.transpose()*SB*A << endl;
  for( int i = 0 ; i < nvec ; i++ ){
    cout << X.col(i).transpose() * A;     // 各ベクトルごとに出力
    cout << " " << lbls[i] << endl;       // クラスラベルを出力
  }
}
