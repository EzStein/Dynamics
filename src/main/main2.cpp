#include "../concurrency/thread_pool.h"
#include <iostream>
#include <thread>
#include <chrono>
int main() {
  thread_pool pool(4);
  for(int i = 0; i != 400; i++) {

    pool.execute([=]()->void{
      for(int i = 0; i != 10000000; ++i) {
        volatile int j = i;
      }
      //std::cout << i << std::endl;
    });
  }
  std::cout << "ABC" << std::endl;
  pool.join();
  pool.terminate();

  std::cout << "ABC" << std::endl;
  return 0;
}
