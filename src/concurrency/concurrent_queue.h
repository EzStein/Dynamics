#ifndef CONCURRENT_QUEUE_GUARD
#define CONCURRENT_QUEUE_GUARD
#include <queue>
#include <mutex>
#include <condition_variable>
#include <deque>

/*This class wraps a std::queue while providing synchronization*/
template<class T, class Container = std::deque<T> >
class concurrent_queue {
public:
  typedef typename std::queue<T, Container>::value_type value_type;
  typedef typename std::queue<T, Container>::size_type size_type;
  typedef typename std::queue<T, Container>::container_type container_type;
  explicit concurrent_queue(const container_type& ctnr = container_type());
  bool empty() const;
  size_type size() const;

  /*pushes the item onto the queue*/
  void push(const value_type&);

  /*Pops an item off the queue or waits if no items are available.
  The result is stored in the provided reference*/
  void pop(value_type&);
private:
  std::mutex mut;
  std::condition_variable condition;
  std::queue<T, Container> queue;
};


template<class T, class Container>
concurrent_queue<T, Container>::concurrent_queue(const typename concurrent_queue<T,Container>::container_type& ctnr) : queue(ctnr) { }

template<class T, class Container>
bool concurrent_queue<T, Container>::empty() const {
  std::lock_guard<std::mutex> lock(mut);
  return queue.empty();
}

template<class T, class Container>
typename concurrent_queue<T, Container>::size_type concurrent_queue<T, Container>::size() const {
  std::lock_guard<std::mutex> lock(mut);
  return queue.size();
}

template<class T, class Container>
void concurrent_queue<T, Container>::push(const typename concurrent_queue<T,Container>::value_type& element) {
  std::lock_guard<std::mutex> lock(mut);
  queue.push(element);
  condition.notify_one();
}

template<class T, class Container>
void concurrent_queue<T, Container>::pop(typename concurrent_queue<T,Container>::value_type& result) {
  std::unique_lock<std::mutex> lock(mut);
  condition.wait(lock, [this]()->bool{
    return !this->queue.empty();
  });
  result = queue.front();
  queue.pop();

}
#endif
