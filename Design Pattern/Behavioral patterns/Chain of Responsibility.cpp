#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// 处理者接口声明了一个创建处理者链的方法。还声明了一个执行请求的方法。
struct ComponentWithContextualHelp {
    virtual ~ComponentWithContextualHelp() = default;
    virtual void showHelp() = 0;
};
struct Container;

// 简单组件的基础类。
struct Component : public ComponentWithContextualHelp {
   protected:
    // 组件容器在处理者链中作为“下一个”链接。
    weak_ptr<Container> container_;

   public:
    string tooltipText;

    Component(string tip) : tooltipText(tip) {}
    void setContainer(const weak_ptr<Container>& container) { container_ = container; }
    void showHelp() override;
};

// 容器可以将简单组件和其他容器作为其子项目。链关系将在这里建立。该类将从
// 其父类处继承 showHelp（显示帮助）的行为。
struct Container : public Component, public enable_shared_from_this<Container> {
   protected:
    vector<shared_ptr<Component>> children_;

   public:
    Container(string tip) : Component(tip) {}
    void add(const shared_ptr<Component>& child) {
        children_.emplace_back(child);
        child->setContainer(weak_from_this());
    }
};

inline void Component::showHelp() {
    if (tooltipText.empty())
        if (auto p = container_.lock(); p != nullptr)
            p->showHelp();
    cout << tooltipText << endl;
}
// 原始组件应该能够使用帮助操作的默认实现...
class Button : public Component {
   public:
    string text;
    Button(string text, string tip) : Component(tip), text(text) {}
};

// 但复杂组件可能会对默认实现进行重写。如果无法以新的方式来提供帮助文字，
// 那组件总是还能调用基础实现的（参见 Component 类）。
class Panel : public Container {
   public:
    string modalHelpText;
    Panel(string text, string tip) : Container(tip), modalHelpText(text) {}

    void showHelp() override {
        if (modalHelpText.empty())
            Container::showHelp();
        else
            cout << modalHelpText << endl;
    }
};
// ...同上...
class Dialog : public Container {
   public:
    string wikiPageURL;
    Dialog(string url, string tip) : Container(tip), wikiPageURL(url) {}

    void showHelp() override {
        if (wikiPageURL.empty())
            Container::showHelp();
        else
            cout << wikiPageURL << endl;
    }
};

struct App {
    App() {
        auto dialog = make_shared<Dialog>("预算报告", "报告的地址...");
        auto panel = make_shared<Panel>("", "本面板用于...");
        auto ok = make_shared<Button>("确认", "这是一个确认按钮...");
        auto cancel = make_shared<Button>("取消", "这是一个取消按钮...");

        panel->add(ok);
        panel->add(cancel);
        dialog->add(panel);

        dialog->showHelp();
        panel->showHelp();
        ok->showHelp();
        cancel->showHelp();
    }
};

int main() {
    App();
    return 0;
}
