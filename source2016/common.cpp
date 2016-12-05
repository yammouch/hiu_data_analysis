// common.cpp
#include "common.h"

double updateWeight( std::vector<std::vector<double>>& vecs, 
                     std::vector<int>& lbls, 
                     std::vector<std::vector<double>>& weight ){
  int nvec = vecs.size();
  int ndim = vecs[0].size();
  int numc = weight.size();
  //-- クラスタの重心として重みベクトルを更新 --
  for( int k = 0 ; k < numc ; k++ )
    for( int m = 0 ; m < ndim ; m++ ) weight[k][m] = 0;
  std::vector<double> n(numc,0);
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls[i];
    n[label]++;
    for( int m = 0 ; m < ndim ; m++ ) weight[label][m] += vecs[i][m];
  }
  for( int k = 0 ; k < numc ; k++ )
    if( n[k] != 0 ) 
      for( int m = 0 ; m < ndim ; m++ ) weight[k][m] *= 1.0 / n[k];
  //-- 2乗誤差（＝クラスタ内平方和Jw）の算出 --
  return SQError( vecs, lbls, weight );
}

double updateLabel( std::vector<std::vector<double>>& vecs, 
                    std::vector<int>& lbls, 
                    std::vector<std::vector<double>>& weight){
  int nvec = vecs.size();
  int ndim = vecs[0].size();
  int numc = weight.size();
  //-- クラスタラベルの更新 --
  for( int i = 0 ; i < nvec ; i++ ){
    int label = -1;        //-- クラスタラベルの仮設定（以下で更新）
    double minE = std::numeric_limits<double>::max(); //最小誤差初期化
    for( int k = 0 ; k < numc ; k++ ){ //-- 各重みベクトルとの誤差計算
      double sum = 0;      //-- 誤差計算用の変数
      for( int m = 0 ; m < ndim ; m++ ){
        double tmp = vecs[i][m] - weight[k][m];
        sum += tmp * tmp;
      }
      if( sum < minE ){    //-- もし「最小誤差」の候補より小さいなら
        minE = sum;        //-- 「最小誤差」の候補の更新
        label = k;         //-- クラスタラベル候補の更新
      }
    }
    lbls[i] = label;       //-- クラスタラベルの更新
  }
  //-- 2乗誤差（＝量子化誤差Eq）の算出 --
  return SQError( vecs, lbls, weight );
}

double SQError( std::vector<std::vector<double>>& vecs, 
                std::vector<int>& lbls, 
                std::vector<std::vector<double>>& weight ){
  double sum = 0;          //-- 2乗誤差計算用の変数
  int nvec = vecs.size();
  int ndim = vecs[0].size();
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls[i];
    for( int m = 0 ; m < ndim ; m++ ){
      double tmp = vecs[i][m] - weight[label][m];
      sum += tmp * tmp;
    }
  }
  return sum;
}
