#include <iostream>
#include <memory>
using namespace std;

class Component;

// 中介者接口声明了一个能让组件将各种事件通知给中介者的方法。中介者可对这
// 些事件做出响应并将执行工作传递给其他组件。
struct Mediator {
  virtual ~Mediator() = default;
  virtual void notify(Component* sender, string_view event) = 0;
};

// 组件会使用中介者接口与中介者进行交互。因此只需将它们与不同的中介者连接
// 起来，你就能在其他情境中使用这些组件了。
class Component {
 public:
  explicit Component(Mediator& dialog) : dialog_{dialog} {}
  virtual ~Component() = default;
  virtual void click() { dialog_.notify(this, "click"); };
  virtual void keypress() { dialog_.notify(this, "keypress"); };

 protected:
  Mediator& dialog_;
};

class Button : public Component {
 public:
  explicit Button(Mediator& dialog) : Component(dialog) {}
};
class Textbox : public Component {
 public:
  explicit Textbox(Mediator& dialog) : Component(dialog) {}
};
class Checkbox : public Component {
 public:
  explicit Checkbox(Mediator& dialog) : Component(dialog) {}
  virtual ~Checkbox() = default;
  virtual void check() {
    checked = !checked;
    dialog_.notify(this, "check");
  }

  bool checked{false};
};

// 具体中介者类可解开各组件之间相互交叉的连接关系并将其转移到中介者中。
class AuthenticationDialog : public Mediator {
 public:
  // 当组件中有事件发生时，它会通知中介者。中介者接收到通知后可自行处理，
  // 也可将请求传递给另一个组件。
  void notify(Component* sender, string_view event) override {
    if (sender == loginOrRegisterChkBx && event == "check") {
      if (loginOrRegisterChkBx->checked) {
        title = "login";
        // 1. 显示登录表单组件。
        // 2. 隐藏注册表单组件。
      } else {
        title = "register";
        // 1. 显示注册表单组件。
        // 2. 隐藏登录表单组件。
      }
      cout << "title->" << title << endl;
    }
    if (sender == okBtn && event == "click") {
      if (loginOrRegisterChkBx->checked) {
        // 尝试找到使用登录信息的用户。
        bool found = true;
        if (!found) {
          // 在登录字段上方显示错误信息。
        }
        cout << "found" << endl;
      } else {
        // 1. 使用注册字段中的数据创建用户账号。
        // 2. 完成用户登录工作。 …
      }
    }
  }

  Checkbox* loginOrRegisterChkBx;
  // Textbox *loginUsername, *loginPassword;
  // Textbox *registrationUsername, *registrationPassword, *registrationEmail;
  Button* okBtn;  //, *cancelBtn;
 private:
  string title;
};

struct App {
  App() {
    AuthenticationDialog dialog;
    auto checkbox = make_unique<Checkbox>(dialog);
    auto button = make_unique<Button>(dialog);
    {
      dialog.loginOrRegisterChkBx = checkbox.get();
      dialog.okBtn = button.get();
    }
    checkbox->check();
    button->click();
  }
};

int main() {
  App();
  return 0;
}
