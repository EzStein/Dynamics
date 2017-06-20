#include <iostream>
#include "../compiler/NFA.h"
#include "../compiler/DFA.h"
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

int main() {

  cout << "Enter a regular expression: ";
  string regex, string;
  cin >> regex;
  cout << "Now enter a string: ";
  cin >> string;

  NFA nfa(regex.c_str());

  bool val = nfa.accepts(string.c_str());
  if(val) {
    cout << "NFA: The regex matches your string!" << endl;
  } else {
    cout << "NFA: No Match!" << endl;
  }
  DFA dfa(nfa);
  val = dfa.accepts(string.c_str());
  if(val) {
    cout << "DFA: The regex matches your string!" << endl;
  } else {
    cout << "DFA: No Match!" << endl;
  }
  return 0;
}
