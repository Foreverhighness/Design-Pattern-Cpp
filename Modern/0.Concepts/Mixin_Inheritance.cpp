#include <iostream>
// Without template parameter.
struct Baz {
    void baz() { std::cout << "baz" << std::endl; }
};

template <class Base>
struct Bar : Base {
    void bar() { std::cout << "bar" << std::endl; }
};

template <class Base>
struct Foo : Base {
    void foo() { std::cout << "foo" << std::endl; }
};

int main() {
    Foo<Bar<Baz>> fooBarBaz;
    fooBarBaz.baz();
    fooBarBaz.bar();
    fooBarBaz.foo();
    return 0;
}