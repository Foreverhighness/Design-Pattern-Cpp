#include <iostream>
#include <thread>
using namespace std;

#include <boost/signals2.hpp>
namespace si = boost::signals2;

template <typename T>
class Observable {
 public:
  si::signal<void(T&, string_view)> property_changed;
};

class Person : public Observable<Person> {
 public:
  int get_age() const { return age_; }
  void set_age(const int age) {
    if (age_ == age)
      return;
    age_ = age;
    property_changed(*this, "age");
  }
  Person(const int age) : age_{age} {}

 private:
  int age_{0};
};

void test() {
  int age = 14;
  Person p{age};
  auto consolePersonObserver = [](auto& source, string_view field_name) {
    cout << "Person's " << field_name << " has changed to ";
    if (field_name == "age")
      cout << source.get_age();
    cout << ".\n";
  };
  auto conn = p.property_changed.connect(consolePersonObserver);
  p.set_age(++age);  // Person's age has changed to 15.
  p.set_age(++age);  // Person's age has changed to 16.
  p.set_age(++age);  // Person's age has changed to 17.
  thread t1{[&conn]() { conn.disconnect(); }};
  thread t2{[&conn]() { conn.disconnect(); }};
  t1.join();
  t2.join();
  p.set_age(++age);  //
}

int main() {
  test();
  return 0;
}
