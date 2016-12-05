// read1D.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int main(int argc, char* argv[]){
  string fname1 = argv[1];
  int i = stoi( argv[2] );
  vector<double> vec;
  double val;
  ifstream ifile1( fname1 );
  while( ifile1 >> val ) vec.emplace_back(val);
  ifile1.close();
  cout << vec[i] << endl;
}
