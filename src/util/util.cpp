#include <cstdlib>
#include "util.h"

using std::string;
void util::strip_white_space(string& str) {
  string::iterator end = str.end();
  for(string::iterator iter = str.begin(); iter != end;) {
    if(isspace(*iter)) {
      /*erase returns iterator to the character imediatly following the erased character*/
      iter = str.erase(iter);
    } else {
      ++iter;
    }
  }
}

double util::string_to_double(const string& str) {
  double ret = 0;
  typedef string::const_iterator iter;
  iter currChar = str.begin();
  iter end = str.end();
  iter begin = str.begin();
  for(; currChar != end; ++currChar) {
    if(*currChar == '.') break;
  }
  /*Save the currChar position*/
  iter decimal = currChar;
  //currChar points to one past the ones digit
  --currChar;
  double multiplier = 1;
  for(; currChar != begin - 1; --currChar) {
    double charVal = *currChar - '0';
    ret += charVal * multiplier;
    multiplier *= 10;
  }
  if(decimal == end) return ret;

  multiplier = 1.0/10.0;
  for(++decimal; decimal != end; ++decimal) {
    double charVal = *decimal - '0';
    ret += charVal * multiplier;
    multiplier /= 10.0;
  }
  return ret;
}

bool util::is_integer(const std::string& s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p;
   std::strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

long util::string_to_long(const std::string& s) {
  return std::strtol(s.c_str(),nullptr,10);
}