// pValSign.cpp 
#include <cmath>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]){
  double n = stod( argv[1] );
  double r = stod( argv[2] );
  double P = pow(0.5,n);
  double pVal = 0;
  for( int i = 0 ; i <= r ; i++ )
    pVal += tgamma(n+1) / (tgamma(i+1)*tgamma(n-i+1)) * P ;
  cout << "p-value= " << pVal << endl;
}
