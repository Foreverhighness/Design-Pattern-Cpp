// clang++ -std=c++2a -Og -Wall lock_free.cpp -o "/project/cpp/.bin/lock_free" -latomic
#include <atomic>
#include <iostream>
struct A {
  float x;
  int y;
  long long z;
};

int main() {
  std::atomic<A> a;
  std::cout << std::boolalpha << a.is_lock_free() << std::endl;
  return 0;
}