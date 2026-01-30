#include <PB2/PB2Util.h>


void PrintHeading(string s) {

  int spaces = (48 - s.size() - 10)/2;
  cout << endl;
  for (int i=0; i<spaces ; i++) {
    cout << " ";
  }
    
  cout << "-----" << s << "-----";
  for (int i=0; i<spaces ; i++) {
    cout << " ";
  }

  cout << endl;
}
