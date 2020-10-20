#include <string>
#include <vector>
using namespace std;
struct HtmlElement {
    string name;
    string text;
    vector<HtmlElement> elem;
public:
    string to_pretty(const int indent = 0) const {

    }
};