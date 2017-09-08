#ifndef THREAD_POOL_GUARD
#define THREAD_POOL_GUARD
#include <functional>
#include <mutex>
#include <thread>
#include "concurrency/concurrent_queue.h"


/*A thread pool that manages several threads by scheduling callables. The thread pool is NOT
concurrent, meaning that any only one thread should access a given instance of this class.
NOTE: if the user does not call join before the thread pool is destructed, the program will terminate.
Also note that it is a mistake to call join before a call to terminate.
*/
class thread_pool {
public:
  /*Constructs a thread pool with the optimal number of threads (at least 1)*/
  thread_pool();

  /*Constructs a thread pool with the provided number of threads*/
  explicit thread_pool(unsigned int);

  /*No copy constructor*/
  thread_pool(const thread_pool&) = delete;

  /*Indicates that the thread_pool will process the remaining functions in its queue, and then stop
  by terminating its threads.
  This method has no effect on currently running functions and will not necessarily cause any threads to terminate.
  After calling this method, subsequent calls to execute will have no effect.*/
  void terminate();


  /*Returns true if joinable has not yet been called.*/
  bool joinable();

  /*Blocks until all threads have been completed. It is a mistake to call this before calling terminate*/
  void join();

  /*Schedules the function to be run with the given arguments by adding it to a queue. This function is non-blocking. This
  function has no effect if terminate was previously called.*/
  template<class Function, class... Args> void execute(Function, Args...);

private:
  concurrent_queue<std::function<void(void)> > queue;
  std::vector<std::thread> threads;
  bool terminateFlag;
  bool joinableFlag;

  void runner();
  void initialize_threads(unsigned int);
};

template<class Function, class... Args> void thread_pool::execute(Function in, Args... args) {
  if(terminateFlag) return;
  std::function<void(void)> func = std::bind(in, args...);
  queue.push(func);
}

#endif
