#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct HtmlElement {
    friend class HtmlBuilder;
    vector<HtmlElement> _elems;
    string _name;
    string _text;
    const size_t indent_size = 2;

    HtmlElement() = default;

   public:
    HtmlElement(string_view name, string_view text) : _name(name), _text(text) {}
    string str(const int indent_num = 0) const {
        ostringstream oss;
        string indent(indent_size * indent_num, ' ');
        oss << indent << "<" << _name << ">" << endl;
        if (_text.size() > 0)
            oss << string(indent_size * (indent_num + 1), ' ') << _text << endl;

        for (const auto& e : _elems)
            oss << e.str(indent_num + 1);

        oss << indent << "</" << _name << ">" << endl;
        return oss.str();
    }
};
class HtmlBuilder {
    HtmlElement _root;

   public:
    explicit HtmlBuilder(string_view name) { _root._name = name; }
    HtmlBuilder& add_child(string_view name, string_view text) {
        _root._elems.emplace_back(name, text);
        return *this;
    }
    HtmlElement build() const { return _root; }
    explicit operator HtmlElement() const { return _root; }
};

void test() {
    // <p>hello</p>
    // auto text = "hello";
    // string output;
    // output += "<p>";
    // output += text;
    // output += "</p>";
    // printf("<p>%s</p>", text);

    // <ul><li>hello</li><li>world</li></ul>
    string words[] = {"hello", "world"};
    ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
        oss << "  <li>" << w << "</li>";
    oss << "</ul>";
    cout << oss.str() << endl;
    // easier
    HtmlBuilder builder{"ul"};
    builder.add_child("li", "hello").add_child("li", "world");
    cout << builder.build().str() << endl;

    // auto builder2 = HtmlElement::build("ul")->add_child_2("li", "hello")->add_child_2("li", "world");
    // cout << builder2 << endl;
}

int main() {
    test();
    return 0;
}
