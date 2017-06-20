#include <thread>
#include <exception>
#include "concurrency/thread_pool.h"

using std::thread;
using std::vector;
using std::function;

thread_pool::thread_pool() : terminateFlag(false), joinableFlag(true) {
  unsigned int threadCount = thread::hardware_concurrency();
  /*If no info was found we choose a happy medium*/
  if(!threadCount) {
    threadCount = 2;
  }
  initialize_threads(threadCount);
}
thread_pool::thread_pool(unsigned int threadCount) : terminateFlag(false), joinableFlag(true) {
  /*If we got zero threads we assume they meant one*/
  if(!threadCount) threadCount = 1;
  initialize_threads(threadCount);
}

void thread_pool::initialize_threads(unsigned int threadCount) {
  for(unsigned int i = 0; i != threadCount; ++i) {
    threads.push_back(thread(&thread_pool::runner, this));
  }
}

void thread_pool::join() {
  if(!terminateFlag) throw std::invalid_argument("Cannot call join() before terminate() on instance of thread_pool.");

  vector<thread>::iterator last = threads.end();
  vector<thread>::iterator thread = threads.begin();

  for(; thread != last; ++thread) {
    thread->join();
  }
  joinableFlag = false;
}

bool thread_pool::joinable() {
  return joinableFlag;
}

void thread_pool::terminate() {
  terminateFlag = true;
  vector<thread>::size_type size = threads.size();
  for(vector<thread>::size_type i = 0; i != size; ++i) {
    queue.push(function<void()>());
  }
}

void thread_pool::runner() {
  while(1) {
    function<void()> func;
    queue.pop(func);
    /*If a function object without a valid target is supplied, we terminate the thread
    * This usually indicates that terminate was called*/
    if(!func) break;
    func();
  }
}
