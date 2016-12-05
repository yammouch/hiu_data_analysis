// pValSignLog.cpp 
#include <cmath>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]){
  double n = stod( argv[1] );
  double r = stod( argv[2] );
  double logP = n * log(0.5);
  double pVal = 0;
  for( int i = 0 ; i <= r ; i++ )
    pVal += exp( lgamma(n+1) - lgamma(i+1) - lgamma(n-i+1) + logP );
  cout << "p-value= " << pVal << endl;
}
