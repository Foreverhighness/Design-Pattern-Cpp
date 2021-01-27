#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
using namespace std;

struct ListStrategy {
  virtual ~ListStrategy() = default;
  virtual void start(ostringstream& oss) {}
  virtual void end(ostringstream& oss) {}
  virtual void add_list_item(ostringstream& oss, string_view item) {}
};

class HtmlListStrategy : public ListStrategy {
 public:
  void start(ostringstream& oss) override { oss << "<ul>\n"; }
  void end(ostringstream& oss) override { oss << "</ul>\n"; }
  void add_list_item(ostringstream& oss, string_view item) override { oss << "<li>" << item << "</li>\n"; }
};

class MarkdownListStrategy : public ListStrategy {
 public:
  void add_list_item(ostringstream& oss, string_view item) override { oss << "- " << item << "\n"; }
};

enum class OutputFormat {
  markdown,
  html,
};

class TextProcessor {
 public:
  void append_list(const vector<string>& items) {
    list_strategy_->start(oss_);
    for (auto&& item : items)
      list_strategy_->add_list_item(oss_, item);
    list_strategy_->end(oss_);
  }
  void set_output_format(const OutputFormat format) {
    switch (format) {
      case OutputFormat::markdown:
        list_strategy_ = make_unique<MarkdownListStrategy>();
        break;
      case OutputFormat::html:
        list_strategy_ = make_unique<HtmlListStrategy>();
        break;
    }
  }
  string str() { return oss_.str(); }
  void clear() { oss_.clear(); }

 private:
  ostringstream oss_;
  unique_ptr<ListStrategy> list_strategy_;
};

void test() {
  {
    TextProcessor tp;
    tp.set_output_format(OutputFormat::markdown);
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
  }
  {
    TextProcessor tp;
    tp.set_output_format(OutputFormat::html);
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
  }
}

int main() {
  test();
  return 0;
}
