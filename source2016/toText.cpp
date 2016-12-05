// toText.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;
int main(int argc, char* argv[]){
  string fname1 = argv[1];
  string fname2 = argv[2];
  map<int,string> toText;
  ifstream ifile2( fname2 );
  string buf,buf2;
  while( getline(ifile2,buf) ){
    istringstream iss(buf);
    string title;
    getline(iss,buf2,'|') ;
    getline(iss,title,'|') ;
    toText.insert(pair<int, string>(stoi(buf2),title));
  }
  ifile2.close();
  ifstream ifile1( fname1 );
  while( getline(ifile1,buf) ){
    istringstream iss(buf);
    int i;
    while( iss >> i ) cout << "{" << toText[i] << "}";
    cout << endl;
  }
  ifile1.close();
}
