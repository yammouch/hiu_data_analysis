// commonDoc.h
#ifndef _COMMONDOC_H
#define _COMMONDOC_H
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <limits>

typedef Eigen::SparseMatrix<double, Eigen::RowMajor> spRMat;
typedef Eigen::SparseMatrix<double> spCMat;
typedef Eigen::MatrixXd Mat;
typedef Eigen::VectorXd Vec;
typedef Eigen::VectorXi Veci;

double updateWeightCos( spCMat& spX, Veci& lbls, Mat& weight );

double updateLabelCos( spCMat& spX, Veci& lbls, Mat& weight );

double aveCos( spCMat& spX, Veci& lbls, Mat& weight );

double updateWeightEntropy( spCMat& spX, Veci& lbls, Mat& weight );

void updateLabelEntropy(spCMat& spX, Veci& lbls, Mat& weight, double a);

double aveEntropy( spCMat& spX, Veci& lbls, Mat& weight );

#endif
