#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;
using coordinate = int;
// 组件接口会声明组合中简单和复杂对象的通用操作。
struct Graphic {
    virtual ~Graphic() = default;
    virtual void move(coordinate x, coordinate y) = 0;
    virtual void draw() = 0;
    virtual void bound(coordinate& top, coordinate& bottom, coordinate& left, coordinate& right) = 0;
};

// 叶节点类代表组合的终端对象。叶节点对象中不能包含任何子对象。叶节点对象
// 通常会完成实际的工作，组合对象则仅会将工作委派给自己的子部件。
class Dot : public Graphic {
   public:
    coordinate x_, y_;
    Dot(coordinate x, coordinate y) : x_(x), y_(y) {}
    void move(coordinate x, coordinate y) override { x_ += x, y_ += y; }
    void draw() override { cout << "Draw dot (" << x_ << ", " << y_ << ")" << endl; }
    void bound(coordinate& top, coordinate& bottom, coordinate& left, coordinate& right) override {
        top = max(top, y_);
        bottom = min(bottom, y_);
        left = min(left, x_);
        right = max(right, x_);
    }
};

// 所有组件类都可以扩展其他组件。
class Circle : public Dot {
   public:
    coordinate radius_;
    Circle(coordinate x, coordinate y, coordinate radius) : Dot(x, y), radius_(radius) {}
    void draw() override {
        cout << "Draw Circle at center(" << x_ << ", " << y_ << "), radius is " << radius_ << ".\n";
        int cnt = 4;
        double alpha = .0;
        for (int i = 0; i < cnt; ++i) {
            cout << "  ";
            Dot(x_ + radius_ * sin(alpha), y_ + radius_ * cos(alpha)).draw();
            alpha += 2 * M_PI / cnt;
        }
    }
    void bound(coordinate& top, coordinate& bottom, coordinate& left, coordinate& right) override {
        top = max(top, y_ + radius_);
        bottom = min(bottom, y_ - radius_);
        left = min(left, x_ - radius_);
        right = max(right, x_ + radius_);
    }
};

// 组合类表示可能包含子项目的复杂组件。组合对象通常会将实际工作委派给子项
// 目，然后“汇总”结果。
class CompoundGraphic : public Graphic {
    vector<shared_ptr<Graphic>> children_;

   public:
    CompoundGraphic() = default;
    void add(shared_ptr<Graphic>&& pg) { children_.emplace_back(std::forward<shared_ptr<Graphic>>(pg)); }
    void remove(shared_ptr<Graphic>& pg) {
        children_.erase(std::remove(children_.begin(), children_.end(), pg), children_.end());
    }
    void move(coordinate x, coordinate y) override {
        for (auto&& graphic : children_) {
            graphic->move(x, y);
        }
    }
    void draw() override {
        cout << "Draw CompoundGraphic: " << endl;
        for (auto&& graphic : children_) {
            graphic->draw();
        }
        if (!children_.empty()) {
            coordinate top = -1e9, bottom = 1e9;
            coordinate left = 1e9, right = -1e9;
            bound(top, bottom, left, right);
            cout << " Draw Rectangle: \n"
                 << "   left: " << left << "   right: " << right << "   top: " << top
                 << "   bottom: " << bottom << "\n";
        }
    }
    void bound(coordinate& top, coordinate& bottom, coordinate& left, coordinate& right) override {
        for (auto&& graphic : children_)
            graphic->bound(top, bottom, left, right);
        top += 1;
        bottom -= 1;
        left -= 1;
        right += 1;
    }
};
struct App {
    void process(Dot& d) {
        d.draw();
        d.move(1, 2);
        d.draw();
    }
    App() {
        Circle c(1, 1, 1);
        process(c);
        cout << "^^^Process^^^\n";

        CompoundGraphic all;

        all.add(make_shared<Dot>(1, 2));
        all.add(make_shared<Circle>(5, 3, 10));
        shared_ptr<CompoundGraphic> child = make_shared<CompoundGraphic>();
        child->add(make_shared<Dot>(2, 1));
        child->move(2, -3);
        child->add(make_shared<Circle>(0, 1, 2));
        child->move(-9, -3);
        child->draw();
        cout << "^^^Child^^^\n";
        all.add(child);
        all.draw();
    }
};

int main() {
    App();
    return 0;
}