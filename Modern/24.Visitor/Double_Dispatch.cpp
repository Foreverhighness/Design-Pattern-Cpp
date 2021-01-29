#include <iostream>
using namespace std;

struct Foo;
struct Bar;
void func(Foo*) { cout << "Foo" << endl; }
void func(Bar*) { cout << "Bar" << endl; }

struct Base {
  virtual ~Base() = default;
  virtual void call() = 0;
};

struct Foo : public Base {
  void call() override { func(this); }
};
struct Bar : public Base {
  void call() override { func(this); }
};

void test() {
  Base&& foo = Foo();
  Base&& bar = Bar();
  foo.call();
  bar.call();
}

int main() {
  test();
  return 0;
}
