#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
using namespace std;

struct LoggerHelper {};

struct Logger {
  function<void()> func;
  string name;
  Logger(const function<void()>& func, string_view name) : func{func}, name(name) {}
  void operator()() const {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};

template <typename Func>
struct Logger1 {
  Func func;
  string name;
  Logger1(const Func& func, string_view name) : func{func}, name(name) {}
  void operator()() const {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};
template <typename R, typename... Args>
struct Logger2;

template <typename R, typename... Args>
struct Logger2<R(Args...)> {
  using Func = function<R(Args...)>;
  Func func;
  string name;
  Logger2(Func func) : func{func}, name{"implicit"} {}
  Logger2(const Func& func, string_view name) : func{func}, name{name} {}
  R operator()(Args&&... args) const {
    cout << "Entering " << name << endl;
    if constexpr (std::is_same_v<R, void>) {
      func(std::forward<Args>(args)...);
      cout << "Exiting " << name << endl;
    } else {
      R result = func(std::forward<Args>(args)...);
      cout << "Exiting " << name << endl;
      return result;
    }
  }
};

double add(double a, double b) {
  cout << a << "+" << b << "=" << (a + b) << endl;
  return a + b;
}
void test() {
  auto log0 = Logger([]() { cout << "Hello" << endl; }, "000");
  log0();

  Logger1 ll{log0, "lll"};
  ll();
  auto log1 = Logger1([]() { cout << "Hello" << endl; }, "111");
  auto log2 = Logger1([]() { cout << "World" << endl; }, "222");
  log1();
  // log1.func = log2.func; // complie error
  log2();

  Logger2<void()> log3{log2, "333"};
  log3();
  Logger2<int(int)> log4{[&](int x) {
                           cout << x << endl;
                           return x;
                         },
                         "444"};
  log4(2);
  Logger2<double(double, double)> log5{add, "555"};
  log5(2, 3);

  Logger2<void()> log6([]() { cout << "Hello" << endl; }, "666");
  Logger2<void()> log7([]() { cout << "World" << endl; }, "777");
  log6();
  log7.func = log6.func;
  log7();
}

int main() {
  test();
  return 0;
}
