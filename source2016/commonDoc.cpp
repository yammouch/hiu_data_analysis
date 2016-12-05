// commonDoc.cpp
#include "commonDoc.h"

double updateWeightCos( spCMat& spX, Veci& lbls, Mat& weight ){
  int nvec = spX.cols();
  int ndim = spX.rows();
  int numc = weight.cols();
  //--クラスタ重心を求め，正規化（L2ノルムを1に）
  weight = Mat::Zero(ndim,numc);
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls(i);
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      weight(it.row(),label) += it.value();
  }
  for( int k = 0 ; k < numc ; k++ ){
    double norm = weight.col(k).norm();
    if( norm != 0.0 ) weight.col(k) /= norm;
  }
  //-- 平均Cosine（クラスタリングの評価値）の算出 --
  return aveCos( spX, lbls, weight );
}

double updateLabelCos( spCMat& spX, Veci& lbls, Mat& weight ){
  int nvec = spX.cols();
  int ndim = spX.rows();
  int numc = weight.cols();
  //--クラスタラベルの更新
  for( int i = 0 ; i < nvec ; i++ ){
    int label = -1;        //-- クラスタラベルの仮設定（以下で更新）
    double maxC = -10;     //-- 最大Cosの初期化（取り得ない小さな値）
    for( int k = 0 ; k < numc ; k++ ){ //-- 各重みベクトルとのCos計算
      double cos = 0;      //-- Cos計算用の変数
      for(spCMat::InnerIterator it(spX,i);it; ++it) 
	cos += weight(it.row(),k) * it.value();
      if( cos > maxC ){    //-- もし「最大Cos」の候補より大きいなら
	maxC = cos;        //-- 「最大Cos」の候補の更新
	label = k;         //-- クラスタラベル候補の更新
      }
    }
    lbls(i) = label;       //-- クラスタラベルの更新
  }
  //-- 平均Cosine（ベクトル量子化の評価値）の算出 --
  return aveCos( spX, lbls, weight );
}

double aveCos( spCMat& spX, Veci& lbls, Mat& weight ){
  int nvec = spX.cols();
  int ndim = spX.rows();
  double aCos = 0;
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls(i);
    double cos = 0;
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      cos += weight(it.row(),label) * it.value();
    aCos += cos;
  }
  return aCos / (double) nvec;
}

double updateWeightEntropy( spCMat& spX, Veci& lbls, Mat& weight ){
  int nvec = spX.cols();
  int ndim = spX.rows();
  int numc = weight.cols();
  //--クラスタ重心を求め，正規化（L1ノルムを1に）
  weight = Mat::Zero(ndim,numc);
  for( int i = 0 ; i < nvec ; i++ ){
    int label = lbls(i);
    for(spCMat::InnerIterator it(spX,i);it; ++it) 
      weight(it.row(),label) += it.value();
  }
  for( int k = 0 ; k < numc ; k++ ){
    double sum = weight.col(k).cwiseAbs().sum();
    if( sum != 0.0 ) weight.col(k) /= sum;
  }
  //-- クラスタ内平均Entropy（クラスタリングの評価値）の算出 --
  return aveEntropy( spX, lbls, weight );
}

void updateLabelEntropy(spCMat& spX, Veci& lbls, Mat& weight, double a){
  int nvec = spX.cols();
  int ndim = spX.rows();
  int numc = weight.cols();
  double a2 = 1.0 - a;
  //--クラスタラベルの更新
  for( int i = 0 ; i < nvec ; i++ ){
    int label = -1;        //-- クラスタラベルの仮設定（以下で更新）
    double minE = std::numeric_limits<double>::max(); //最小距離初期化
    for( int k = 0 ; k < numc ; k++ ){ //-- 各重みベクトルとのsKL計算
      double sKL = 0;      //-- skew(KL)Div.計算用の変数
      for(spCMat::InnerIterator it(spX,i);it; ++it){ 
	double tmp = a*weight(it.row(),k) + a2*it.value();
	sKL -= it.value() * log2(tmp);
      }
      if( sKL < minE ){    //-- もし「最小距離」の候補より小さいなら
	minE = sKL;        //-- 「最小距離」の候補の更新
	label = k;         //-- クラスタラベル候補の更新
      }
    }
    lbls(i) = label;       //-- クラスタラベルの更新
  }
}

double aveEntropy( spCMat& spX, Veci& lbls, Mat& weight ){
  int nvec = spX.cols();
  int ndim = spX.rows();
  int numc = weight.cols();
  double aEntropy = 0;
  std::vector<double> nelem(numc,0);
  for( int i = 0 ; i < nvec ; i++ ) nelem[lbls(i)]++;
  for( int k = 0 ; k < numc ; k++ ){
    double sum = 0;
    for( int m = 0 ; m < ndim ; m++ )
      if( weight(m,k) > 0 ) sum -= weight(m,k) * log2(weight(m,k));
    aEntropy += sum * nelem[k];
  }
  return aEntropy / (double) nvec;
}
