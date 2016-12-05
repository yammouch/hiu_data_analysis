// updateVocabulary.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;
int main(int argc, char* argv[]){
  string fnameVoca = argv[1];
  string fnameStop = argv[2];
  string buf;
  set<string> stopWords;
  ifstream ifile( fnameStop );  
  while( ifile >> buf ) stopWords.insert(buf);
  ifile.close();
  ifile.open( fnameVoca );
  while( ifile >> buf )
    if( stopWords.find(buf) == stopWords.end() )//ストップワードでなければ
      cout << buf << endl;
  ifile.close();
}
