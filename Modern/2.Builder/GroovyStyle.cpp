#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Tag {
    string name;
    string text;
    vector<Tag> children;
    vector<pair<string, string>> attributes;
    const size_t indent_size = 2;
    friend ostream& operator<<(ostream& os, const Tag& tag);

   protected:
    Tag(string_view name, string_view text) : name(name), text(text) {}
    Tag(string_view name, const vector<Tag>& children) : name(name), children(children) {}
};

ostream& operator<<(ostream& os, const Tag& tag) {
    os << "<" << tag.name;
    for (auto&& attr : tag.attributes) {
        os << " " << attr.first << "=\"" << attr.second << "\"";
    }

    if (tag.children.empty()) {
        os << "/>" << endl;
    } else {
        os << ">" << endl;

        if (tag.text.length()) {
            os << tag.text << endl;
        }

        for (auto&& child : tag.children) {
            os << child;
        }
        os << "</" << tag.name << ">" << endl;
    }
    return os;
}

struct P : Tag {
   public:
    explicit P(string_view text) : Tag("P", text) {}
    P(initializer_list<Tag> children) : Tag("P", children) {}
};

struct IMG : Tag {
   public:
    explicit IMG(string_view url) : Tag("img", "") { attributes.emplace_back("src", url); }
};

int main() {
    cout << P{IMG{"http://pokemon.com/pikachu.png"}} << endl;
    return 0;
}