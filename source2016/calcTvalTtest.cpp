// calcTvalTtest.cpp t検定における上側確率計算pに対する検定統計量tの算出
#include <iostream>
#include <boost/math/special_functions/beta.hpp>
using namespace std;
int main(int argc, char* argv[]){
  double df = stod( argv[1] );
  double p  = stod( argv[2] );
  double x  = boost::math::ibeta_inv(0.5*df,0.5,2.0*p);
  double t  = sqrt(df*(1.0-x)/x);
  cout << "df,t,p-value = " << df << ", " << t << ", " << p << endl;
}
