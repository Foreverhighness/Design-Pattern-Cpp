#include <initializer_list>
#include <iostream>
using namespace std;

template <typename T, typename... Ts>
auto printf3(T value, Ts... args) {
  std::cout << value << std::endl;
  (void)std::initializer_list<T>{(
      [&]() {
        std::cout << args << std::endl;
        return 0;
      }(),
      value)...};
}

template <typename T>
int pr(T&& t) {
  cout << t << endl;
  return 0;
}

template <typename... Args>
void VTPrint(Args&&... args) {
  int tmp[] = {pr(std::forward<Args>(args))...};
}
int main() {
  printf3(1, 2, "123", 1.1);
  VTPrint(1, 2, "213,", 1.23);
  return 0;
}