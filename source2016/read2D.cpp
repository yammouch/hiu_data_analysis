// read2D.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int main(int argc, char* argv[]){
  string fname1 = argv[1];
  int i = stoi( argv[2] );
  vector<vector<double>> vecs;
  double x, y;
  ifstream ifile1( fname1 );
  while( ifile1 >> x >> y ){
    vector<double> vec = {x,y};
    vecs.emplace_back(vec.begin(),vec.end());
  }
  ifile1.close();
  cout << vecs[i][0] << " " << vecs[i][1] << endl;
}
