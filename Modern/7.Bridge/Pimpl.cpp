#include <iostream>
#include <memory>
using namespace std;

struct Person {
    void greet();

    Person(string_view name);
    ~Person();
    struct PersonImpl;

   protected:
    string name_;
    shared_ptr<PersonImpl> personImpl_;
};

struct Person::PersonImpl {
    string getName(Person& p) { return p.name_; }
    void greet(Person& p) { cout << "Hello, " << p.name_ << "!" << endl; }
};
Person::Person(string_view name) : name_(name), personImpl_(make_shared<PersonImpl>()) {}
Person::~Person() = default;
void Person::greet() {
    personImpl_->greet(*this);
}

void test() {
    Person p{"world"};
    p.greet();
}

int main() {
    test();
    return 0;
}