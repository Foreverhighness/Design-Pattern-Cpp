#include <iostream>
using namespace std;

void f(int x) { cout << "重载1" << endl; }
void f(int&& x) { cout << "重载2" << endl; }

int main() {
  int&& ri = 5;      // 5是右值，ri是右值引用，ri是左值
//f(5);              // 编译失败，5是右值，即可以被int绑定也可以被int&&绑定
  f(ri);             // ri是左值，只可以被int绑定，无法被int&&绑定，选择重载1
//f(std::move(ri));  // 编译失败，std::move(ri)是右值，既可以被int绑定也可以被int&&绑定
  return 0;
}