// Effective modern C++ Item 30
#include <iostream>
using namespace std;

// static const -> link error
// inline static const -> complied (C++17)
// static constexpr -> complied (C++17)
// inline static constexpr -> complied (C++17)
class Widget {
 public:
  // clang-format off

  inline
  static
  const
  // constexpr
  size_t k_sz = 1024;

  // clang-format on
};

void f(size_t val) { cout << "t: " << val << endl; }

template <typename T>
void fwd(T&& arg) {
  cout << "fwd-> ";
  f(std::forward<T>(arg));
}

int main() {
  f(Widget::k_sz);
  fwd(Widget::k_sz);
  return 0;
}
