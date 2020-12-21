#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class BetterFormattedText {
 public:
  struct TextRange {
    size_t start, end;
    bool capitalize = false;
    // other options here, e.g. bold, italic, etc.
    TextRange(size_t start, size_t end) : start{start}, end{end} {};

    bool covers(size_t position) const { return start <= position && position <= end; }
    bool apply(char& ch) const {
      if (capitalize)
        ch = toupper(ch);
      return true;
    }
  };
  explicit BetterFormattedText(string_view plainText) : plainText_{plainText} {}
  void capitalize(size_t start, size_t end) { get_range(start, end).capitalize = true; }
  TextRange& get_range(size_t start, size_t end) {
    formatting_.emplace_back(start, end);
    return formatting_.back();
  }
  friend ostream& operator<<(ostream& os, const BetterFormattedText& obj) {
    string s;
    for (size_t i{0}; i < obj.plainText_.length(); ++i) {
      char ch{obj.plainText_[i]};
      for (const auto& range : obj.formatting_)
        if (range.covers(i))
          range.apply(ch);
      s += ch;
    }
    return os << s;
  }

 private:
  string plainText_;
  vector<TextRange> formatting_;
};

void test() {
  BetterFormattedText bft("This is a brave new world");
  bft.capitalize(10, 15);
  cout << bft << endl;
  // prints "This is a BRAVE new world"
}

int main() {
  test();
  return 0;
}