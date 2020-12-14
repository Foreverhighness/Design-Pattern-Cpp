#include <iostream>
#include <memory>
using namespace std;

// #include <boost/lexical_cast.hpp>  // comment this header to use std::to_string;
// #ifdef BOOST_LEXICAL_CAST_INCLUDED
// #define to_string boost::lexical_cast<std::string>
// #endif

template <typename T>
struct Foo : public T {
  template <typename U>
  Foo(const U& u) : T{u} {}

  template <typename... Args>
  auto operator()(Args&&... args) const {
    cout << "Entering "
         << "Foo" << endl;
    if constexpr (std::is_same_v<std::invoke_result_t<T, Args...>, void>) {
      T::operator()(std::forward<Args>(args)...);
      cout << "Exiting "
           << "Foo" << endl;
    } else {
      auto ret = T::operator()(std::forward<Args>(args)...);
      cout << "Exiting "
           << "Foo" << endl;
      return ret;
    }
  }
};

template <typename T>
struct Bar : public T {
  template <typename U>
  Bar(const U& u) : T{u} {}

  template <typename... Args>
  auto operator()(Args&&... args) const {
    cout << "Entering "
         << "Bar" << endl;
    if constexpr (std::is_same_v<std::invoke_result_t<T, Args...>, void>) {
      T::operator()(std::forward<Args>(args)...);
      cout << "Exiting "
           << "Bar" << endl;
    } else {
      auto ret = T::operator()(std::forward<Args>(args)...);
      cout << "Exiting "
           << "Bar" << endl;
      return ret;
    }
  }
};

struct Baz {
  void operator()(int x) const { cout << x << "\nBaz" << endl; }
};

void test() {
  Foo<Bar<Baz>> f{Baz()};
  f(5);
}

int main() {
  test();
  return 0;
}