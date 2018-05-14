#ifndef __SAFE_QUEUE_H
#define __SAFE_QUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <stdarg.h>

template<class T>
class ThreadSafeQueue {
private:
  std::queue<T> qu;
  std::mutex m;
public:
  bool push(T elem) {
    if (elem == nullptr) {
      return false;
    }
    std::unique_lock<std::mutex> lock(m);
    qu.push(elem);
    return true;
  }

  bool emplace(int n, T elem, ...) {
    if (n <= 0) {
      return false;
    }
    va_list targs;
    std::unique_lock<std::mutex> lock(m);

    va_start(targs, n);
    for (int i = 0; i < n; i++) {
      T& t = va_arg(targs, T);
      if (t != nullptr) {
        qu.push(t);
      }
    }
    va_end(targs);
    return true;
  }

  bool next(T& elem) {
    std::unique_lock<std::mutex> lock(m);
    if (qu.empty()) {
      return false;
    }
    elem = qu.front();
    qu.pop();
    return true;
  }
};

#endif
