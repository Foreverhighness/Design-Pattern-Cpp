#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

class MarkdownListStrategy {
 public:
  void start(ostringstream& oss) {}
  void end(ostringstream& oss) {}
  void add_list_item(ostringstream& oss, string_view item) { oss << "- " << item << "\n"; }
};

class HtmlListStrategy {
 public:
  void start(ostringstream& oss) { oss << "<ul>\n"; }
  void end(ostringstream& oss) { oss << "</ul>\n"; }
  void add_list_item(ostringstream& oss, string_view item) { oss << "<li>" << item << "</li>\n"; }
};

template <typename LS>
class TextProcessor {
 public:
  void append_list(const vector<string>& items) {
    list_strategy_.start(oss_);
    for (auto&& item : items)
      list_strategy_.add_list_item(oss_, item);
    list_strategy_.end(oss_);
  }
  string str() { return oss_.str(); }
  void clear() { oss_.clear(); }

 private:
  ostringstream oss_;
  LS list_strategy_;
};

void test() {
  // markdown
  TextProcessor<MarkdownListStrategy> tpm;
  tpm.append_list({"foo", "bar", "baz"});
  cout << tpm.str();
  // html
  TextProcessor<HtmlListStrategy> tph;
  tph.append_list({"foo", "bar", "baz"});
  cout << tph.str();
}

int main() {
  test();
  return 0;
}
