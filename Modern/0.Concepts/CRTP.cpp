#include <iostream>
#include <memory>
#include <string>
#include <vector>
template <typename Vec>
struct Foo {
    void pushInt(int i) { static_cast<Vec*>(this)->push_back(i); }
    std::string foo() {
        if (static_cast<Vec*>(this)->empty())
            return "[]";
        std::string ret;
        for (auto&& elem : *static_cast<Vec*>(this))
            ret += std::to_string(elem) + ", ";
        ret = "[" + ret.substr(0, ret.length() - 2) + "]";
        return ret;
    }
};

struct VecInt : public std::vector<int>, Foo<VecInt> {};

int main() {
    std::unique_ptr<Foo<VecInt>> list{new Foo<VecInt>()};
    for (int i = 0; i < 10; ++i)
        list->pushInt(i);
    std::cout << list->foo() << std::endl;
    return 0;
}