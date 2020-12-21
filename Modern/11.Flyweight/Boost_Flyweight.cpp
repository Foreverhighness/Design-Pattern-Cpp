#include <iostream>
using namespace std;

#include <boost/flyweight.hpp>
using boost::flyweight;

class User {
 public:
  User(string_view first_name, string_view last_name) : first_name_{first_name}, last_name_{last_name} {}
  string_view get_first_name() const { return first_name_.get(); }
  string_view get_last_name() const { return last_name_.get(); }
  friend ostream& operator<<(ostream& os, const User& obj) {
    return os << "first_name: " << obj.get_first_name() << "\t"
              << "last_name: " << obj.get_last_name();
  }
  void print() const { cout << "(" << &first_name_.get() << " " << &last_name_.get() << ")\n"; }

 private:
  flyweight<string> first_name_, last_name_;
};

void test() {
  User u1{"John", "Smith"};
  User u2{"John", "Canny"};
  User u3{"June", "Smith"};

  cout << u1 << "\n" << u2 << "\n" << u3 << "\n";
  u1.print();
  u2.print();
  u3.print();
}

int main() {
  test();
  return 0;
}