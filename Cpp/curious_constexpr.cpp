#include <iostream>
using namespace std;
constexpr int fibonacci(const int n) {
  constexpr int mod = 1e9 + 7;
  return n == 0 || n == 1 ? 1 : (fibonacci(n - 1) + fibonacci(n - 2)) % mod;
}
constexpr int fib(const int n) {
  constexpr int mod = 1e9 + 7;
  if (n == 0 || n == 1)
    return 1;
  int a = 1, b = 1;
  for (int i = 0; i < n; ++i) {
    int c = (a + b) % mod;
    a = b;
    b = c;
  }
  return b;
}
int main() {
  constexpr int N = 1e4;
  cout << fib(N) << endl;
  if constexpr (static_cast<bool>(fib(N))) {
    cout << "Yes" << endl;
  } else {
    cout << "No" << endl;
  }
  return 0;
}