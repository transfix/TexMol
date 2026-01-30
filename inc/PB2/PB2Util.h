#ifndef ___PB2UTIL_H__
#define ___PB2UTIL_H__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void PrintHeading(string s);

template <class T>
void PrintValue(ofstream & fout, string s, T v) {
  fout << s;
  for(int i=0; i<48-s.size(); i++) {
    fout << '.';
  }
  fout << " " << v << endl;
}

template <class T>
void PrintValue(string s, T v) {
  cout << s;
  for(int i=0; i<48-s.size(); i++) {
    cout << '.';
  }
  cout << " " << v << endl;
}

#endif
