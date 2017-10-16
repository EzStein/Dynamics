#ifndef REG_DEF_GUARD
#define REG_DEF_GUARD
#include <set>
#include <list>
//Provides useful definitions for regular expressions library
typedef unsigned long state_type;
typedef std::set<state_type> state_collection_type;

/*Given two sorted sets, this function determines if they intersect*/
//CREDIT: https://stackoverflow.com/questions/29419922/fastest-way-to-find-out-if-two-sets-overlap
template<class Iterator1, class Iterator2>
bool intersects(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2) {
  while(begin1 != end1 && begin2 != end2) {
    if(*begin1 < *begin2) {
      ++begin1;
      continue;
    }
    if(*begin2 < *begin1) {
      ++begin2;
      continue;
    }
    return true;
  }
  return false;
}

template<class T>
std::set<T> intersect(std::set<T> set1, std::set<T> set2) {
  std::set<T> set;
  typename std::set<T>::const_iterator begin1 = set1.begin();
  typename std::set<T>::const_iterator begin2 = set2.begin();
  typename std::set<T>::const_iterator end1 = set1.end();
  typename std::set<T>::const_iterator end2 = set2.end();

  while(begin1 != end1 && begin2 != end2) {
    if(*begin1 < *begin2) {
      ++begin1;
      continue;
    }
    if(*begin2 < *begin1) {
      ++begin2;
      continue;
    }
    set.insert(*begin1);
    ++begin1;
    ++begin2;
  }
  return set;
}
#endif
