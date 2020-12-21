#include <boost/bimap.hpp>
#include <iostream>
using namespace std;
using boost::bimap;
using KeyType = uint32_t;

class User {
 public:
  User(string_view first_name, string_view last_name) : first_name_{add(first_name)}, last_name_{add(last_name)} {}
  string_view get_first_name() const { return names_.left.find(first_name_)->second; }
  string_view get_last_name() const { return names_.left.find(last_name_)->second; }
  friend ostream& operator<<(ostream& os, const User& obj) {
    return os << "first_name: " << obj.get_first_name() << "\t"
              << "last_name: " << obj.get_last_name();
  }
  void print() const { cout << "(" << first_name_ << ", " << last_name_ << ")\n"; }

 private:
  KeyType first_name_, last_name_;
  inline static bimap<KeyType, string> names_;
  inline static KeyType seed_{};
  static KeyType add(string_view s) {
    if (auto it{names_.right.find(s.data())}; it != names_.right.end())
      return it->second;
    names_.insert({++seed_, s.data()});
    return seed_;
  }
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