#include <iostream>
#include "NFA.h"
#include <set>
using namespace std;

ostream& operator<<(ostream& out, const set<unsigned int>& myset) {
  out << '{';
  for(set<unsigned int>::const_iterator i = myset.begin(); i!=myset.end(); ++i) {
    out << *i << ", ";
  }
  out << '}';
  return out;
}

int mains() {

  cout << "Enter a regular expression: ";
  string regex, string;
  cin >> regex;
  cout << "Now enter a string: ";
  cin >> string;

  NFA nfa(regex.c_str());
  bool val = nfa.accepts(string.c_str());
  if(val) {
    cout << "The regex matches your string!" << endl;
  } else {
    cout << "No Match!" << endl;
  }
  return 0;
}
