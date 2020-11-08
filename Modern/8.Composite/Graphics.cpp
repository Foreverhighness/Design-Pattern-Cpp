#include <iostream>
#include <memory>
#include <vector>
using namespace std;

struct GraphicObject {
    virtual ~GraphicObject() = default;
    virtual void draw() = 0;
};

class Circle : public GraphicObject {
   public:
    void draw() override { cout << "Circle" << endl; }
};

class Group : public GraphicObject {
   protected:
    string name_;

   public:
    vector<unique_ptr<GraphicObject>> objects_;
    explicit Group(string_view name) : name_(name) {}
    void draw() override {
        cout << "Group " << name_ << " contains:" << endl;
        for (auto&& o : objects_)
            o->draw();
    }
};

void test() {
    auto root{make_unique<Group>("root")};
    root->objects_.push_back(make_unique<Circle>());

    auto subgroup{make_unique<Group>("sub")};
    subgroup->objects_.push_back(make_unique<Circle>());

    root->objects_.push_back(std::move(subgroup));

    root->draw();
}
int main() {
    test();
    return 0;
}
