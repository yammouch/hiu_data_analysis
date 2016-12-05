// calcPvalTtest.cpp t検定における検定統計量tに対する上側確率計算pの算出
#include <iostream>
#include <boost/math/special_functions/beta.hpp>
using namespace std;
int main(int argc, char* argv[]){
  double df = stod( argv[1] );
  double t  = stod( argv[2] );
  double x  = df / (t*t+df);
  double p  = 0.5*boost::math::ibeta(0.5*df,0.5,x);
  cout << "df,t,p-value = " << df << ", " << t << ", " << p << endl;
}
