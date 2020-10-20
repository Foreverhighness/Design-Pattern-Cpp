#include <cstdio>
#include <memory>
#include <string>
using std::string;
using std::unique_ptr;

struct Button {
    virtual void render() = 0;
    virtual void onClick() = 0;
    virtual ~Button() { puts("Droping"); }
};

class WindowButton : Button {
    friend class WindowDialog;

    void render() override { puts("Render from window."); }
    void onClick() override { puts("Click from window."); }
};

class WebButton : Button {
    friend class WebDialog;

    void render() override { puts("Render from web."); }
    void onClick() override { puts("Click from web."); }
};

struct Dialog {
    virtual unique_ptr<Button> createButton() = 0;

    void render() {
        unique_ptr<Button> okButton = createButton();

        okButton->render();
        okButton->onClick();
    }
};

class WindowDialog : public Dialog {
    unique_ptr<Button> createButton() { return unique_ptr<Button>(new WindowButton()); }
};

class WebDialog : public Dialog {
    unique_ptr<Button> createButton() { return unique_ptr<Button>(new WebButton()); }
};

class App {
    unique_ptr<Dialog> dialog;

   public:
    App(const string& name) {
        if (name == "web")
            dialog = unique_ptr<Dialog>(new WebDialog());
        else if (name == "window")
            dialog = unique_ptr<Dialog>(new WindowDialog());
        else
            throw std::logic_error("Unsupport type!");
        dialog->render();
    }
};

int main() {
    { App("web"); }
    { App("window"); }
    { App("error"); }
    return 0;
}