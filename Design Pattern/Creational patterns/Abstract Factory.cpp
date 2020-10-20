// 抽象工厂接口声明了一组能返回不同抽象产品的方法。这些产品属于同一个系列
// 且在高层主题或概念上具有相关性。同系列的产品通常能相互搭配使用。系列产
// 品可有多个变体，但不同变体的产品不能搭配使用。
#define NDEBUG
#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
using std::string;
using std::unique_ptr;

struct Button {
    virtual ~Button(){};
    virtual void paint() = 0;
};

struct CheckBox {
    virtual ~CheckBox(){};
    virtual void paint() = 0;
};

class MacButton : public Button {
    void paint() override { puts("Paint a button on Mac;"); }
};

class MacCheckBox : public CheckBox {
    void paint() override { puts("Paint a checkbox on Mac;"); }
};

class WindowsButton : public Button {
    void paint() override { puts("Paint a button on Windows;"); }
};

class WindowsCheckBox : public CheckBox {
    void paint() override { puts("Paint a checkbox on Windows;"); }
};

struct GUIFactory {
    virtual ~GUIFactory(){};
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<CheckBox> createCheckBox() = 0;
};

class MacFactory : public GUIFactory {
    unique_ptr<Button> createButton() override { return unique_ptr<Button>(new MacButton()); }
    unique_ptr<CheckBox> createCheckBox() override { return unique_ptr<CheckBox>(new MacCheckBox()); }
};

class WindowsFactory : public GUIFactory {
    unique_ptr<Button> createButton() override { return unique_ptr<Button>(new WindowsButton()); }
    unique_ptr<CheckBox> createCheckBox() override { return unique_ptr<CheckBox>(new WindowsCheckBox()); }
};

class App {
    unique_ptr<GUIFactory> factory;

   public:
    App(const string& name) {
        if (name == "windows")
            factory = unique_ptr<GUIFactory>(new WindowsFactory());
        else if (name == "mac")
            factory = unique_ptr<GUIFactory>(new MacFactory());
        else
            assert(false && "Unsupport type.");
        if (!factory) return;
        factory->createButton()->paint();
        factory->createCheckBox()->paint();
    }
};

int main() {
    { App("windows"); }
    { App("mac"); }
    { App("none"); }
    return 0;
}