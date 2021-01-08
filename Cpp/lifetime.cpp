#include <iostream>
using namespace std;
struct A {
  A() : log{"default"} { cout << "A\n"; }
  A(const A&) : log{"copy"} { cout << "A&\n"; }
  A(A&&) : log{"move"} { cout << "A&&\n"; }
  ~A() { cout << "~A\n"; }

  string log{"Default"};
};

A create() {
  A a;
  return a;
}

A create_with_move() {
  A a;
  return std::move(a);  // warnning: move a local object.
}

A&& create_rref() {
  A a;
  return std::move(a); // bug.
}

A&& create_rref_without_move() {
  return A();  // bug, warning: return reference to local temporary object.
}

int main() {
  {
    cout << "A create:\n";
    auto a = create();
    cout << a.log << "\n";
  }
  cout << "-----\n";
  {
    cout << "A create_with_move:\n";
    auto a = create_with_move();
    cout << a.log << "\n";
  }
  cout << "-----\n";
  {
    cout << "A&& create_rref\n";
    auto&& a = create_rref();
    cout << a.log << "\n";
  }
  cout << "-----\n";
  {
    cout << "A&& create_rref_without_move\n";
    const auto& a = create_rref_without_move();
    cout << a.log << "\n";
  }
  cout << "-----\n";
  cout << "End\n";
}