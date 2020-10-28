// use clang++ -fdeclspec -std=c++2a -Og -Wall Properties.cpp -o Properties
#include <iostream>
using namespace std;
class Person {
    int age_;

   public:
    int get_age() const {
        cout << "Get age " << age_ << endl;
        return age_;
    }
    void set_age(int value) {
        cout << "Set age to " << value << endl;
        age_ = value;
    }
    __declspec(property(get = get_age, put = set_age)) int age;
};
void test() {
    Person p;
    p.age = 20;
    cout << p.age << endl;
}
int main() {
    test();
    return 0;
}