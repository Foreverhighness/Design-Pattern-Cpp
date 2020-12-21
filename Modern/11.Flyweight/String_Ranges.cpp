#include <array>
#include <iostream>
#include <memory>
using namespace std;

class FormattedText {
 public:
  explicit FormattedText(string_view plainText) : plainText_{plainText}, caps_{new bool[plainText_.length()]} {
    fill(caps_.get(), caps_.get() + plainText_.length(), false);
  }
  void capitalize(size_t start, size_t end) {
    for (size_t i{start}; i < end; ++i) {
      caps_[i] = true;
    }
  }
  friend ostream& operator<<(ostream& os, const FormattedText& obj) {
    string s;
    for (size_t i{0}; i < obj.plainText_.length(); ++i) {
      char ch{obj.plainText_[i]};
      s += obj.caps_[i] ? toupper(ch) : ch;
    }
    return os << s;
  }

 private:
  string plainText_;
  unique_ptr<bool[]> caps_;
};

void test() {
  FormattedText ft("This is a brave new world");
  ft.capitalize(10, 15);
  cout << ft << endl;
  // prints "This is a BRAVE new world"
}

int main() {
  test();
  return 0;
}