// evalClustering.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "commonDoc.h"
using namespace Eigen;
using namespace std;
int main(int argc, char* argv[]){
  string fnameA = argv[1]; // （カテゴリ／クラスラベル）ファイル
  string fnameC = argv[2]; // （クラスタラベル）ファイル
  ifstream ifileA( fnameA );
  ifstream ifileC( fnameC );
  string buf;
  vector<int> vecA,vecC;
  while( getline(ifileA,buf) ) vecA.emplace_back(stoi(buf)-1);
  while( getline(ifileC,buf) ) vecC.emplace_back(stoi(buf)-1);
  ifileA.close();
  ifileC.close();
  int numA = *(max_element(vecA.cbegin(),vecA.cend())) +1; // カテゴリ数
  int numC = *(max_element(vecC.cbegin(),vecC.cend())) +1; // クラスタ数
  double nvec = vecA.size();
  Mat cross = MatrixXd::Zero(numC,numA);
  for( int i = 0 ; i < nvec ; i++ ) cross(vecC[i],vecA[i])++;
  Vec crossA = cross.colwise().sum(); // 各カテゴリの文書数
  Vec crossC = cross.rowwise().sum(); // 各クラスタの文書数
  //-- purity
  double purity = 0; //「クラスタに属する最大カテゴリの文書数」の和
  for( int k = 0 ; k < numC ; k++ ) purity += cross.row(k).maxCoeff();
  purity /= nvec;
  //-- RIとF
  double TP=0, TPFN=0, TPFP=0, All=0;
  for( int k = 0 ; k < numC ; k++ )
    for( int j = 0 ; j < numA ; j++ ) 
      TP += cross(k,j) * (cross(k,j)-1) / 2;
  for(int j = 0 ; j < numA ; j++) TPFN += crossA(j)*(crossA(j)-1)/2;
  for(int k = 0 ; k < numC ; k++) TPFP += crossC(k)*(crossC(k)-1)/2;
  All = nvec * (nvec - 1.0) / 2;
  double RI = (All - TPFN - TPFP + 2*TP) / All;
  double  F = (TP/TPFN * TP/TPFP)*2.0 / (TP/TPFN + TP/TPFP);
  //-- NMI 
  double Hc=0, Ha=0, I=0;
  for( int k = 0 ; k < numC ; k++ )
    for( int j = 0 ; j < numA ; j++ )
      if( cross(k,j) != 0 ) 
	I +=cross(k,j)/nvec*log2(cross(k,j)*nvec/(crossC(k)*crossA(j)));
  for( int k = 0 ; k < numC ; k++ )
    if( crossC(k) != 0 ) Hc -= crossC(k)/nvec * log2(crossC(k)/nvec);
  for( int j = 0 ; j < numA ; j++ )
    if( crossA(j) != 0 ) Ha -= crossA(j)/nvec * log2(crossA(j)/nvec);
  double NMI = I / ((Hc + Ha)/2);
  //-- 出力
  cout << "purity = " << purity << endl;
  cout << "RI     = " << RI << endl;
  cout << "F      = " << F << endl;
  cout << "NMI    = " << NMI << endl;
}
