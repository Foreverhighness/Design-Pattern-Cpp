#include <iostream>
#include <memory>
#include <stack>
#include <vector>
using namespace std;

// 编辑器类包含实际的文本编辑操作。它会担任接收者的角色：最后所有命令都会
// 将执行工作委派给编辑器的方法。
class Editor {
   public:
    string text;
    string getSelection() const { return text; }
    void deleteSelection() { text.clear(); }
    void replaceSelection(string_view text) { this->text = text; }
};
struct Command;
class CommandHistory;
class Application {
   public:
    string clipboard;
    vector<shared_ptr<Editor>> editors;
    weak_ptr<Editor> activeEditor;
    unique_ptr<CommandHistory> history;
    explicit Application(size_t n = 1);
    void createUI();
    void executeCommand(unique_ptr<Command> command);
    void undo();
};
// 命令基类会为所有具体命令定义通用接口。
struct Command {
    virtual ~Command() = default;
    // 执行方法被声明为抽象以强制所有具体命令提供自己的实现。该方法必须根
    // 据命令是否更改编辑器的状态返回 true 或 false。
    virtual bool execute() = 0;

    explicit Command(Application& app, const weak_ptr<Editor>& editor) : app(app), editor(editor) {}
    // 备份编辑器状态。
    void saveBackup() {
        if (auto p_editor = editor.lock(); p_editor)
            backup = p_editor->text;
    }
    // 恢复编辑器状态。
    void undo() {
        if (auto p_editor = editor.lock(); p_editor)
            p_editor->text = backup;
    }

   protected:
    Application& app;
    weak_ptr<Editor> editor;
    string backup;
};

// 这里是具体命令。
class CopyCommand : public Command {
   public:
    explicit CopyCommand(Application& app, const weak_ptr<Editor>& editor) : Command(app, editor) {}
    // 复制命令不会被保存到历史记录中，因为它没有改变编辑器的状态。
    bool execute() override {
        if (auto p_editor = editor.lock(); p_editor) {
            cout << "Copy " << p_editor->getSelection() << " to clipboard.\n";
            app.clipboard = p_editor->getSelection();
        }
        return false;
    }
};
class CutCommand : public Command {
   public:
    explicit CutCommand(Application& app, const weak_ptr<Editor>& editor) : Command(app, editor) {}
    // 剪切命令改变了编辑器的状态，因此它必须被保存到历史记录中。只要方法
    // 返回 true，它就会被保存。
    bool execute() override {
        if (auto p_editor = editor.lock(); p_editor) {
            saveBackup();
            cout << "Cut " << p_editor->getSelection() << " to clipboard.\n";
            app.clipboard = p_editor->getSelection();
            p_editor->deleteSelection();
            return true;
        }
        return false;
    }
};
class PasteCommand : public Command {
   public:
    explicit PasteCommand(Application& app, const weak_ptr<Editor>& editor) : Command(app, editor) {}
    bool execute() override {
        if (auto p_editor = editor.lock(); p_editor) {
            saveBackup();
            cout << "Paste " << app.clipboard << " to editor.\n";
            p_editor->replaceSelection(app.clipboard);
            return true;
        }
        return false;
    }
};
// 撤销操作也是一个命令。
class UndoCommand : public Command {
   public:
    explicit UndoCommand(Application& app, const weak_ptr<Editor>& editor) : Command(app, editor) {}
    bool execute() override {
        cout << "Undo.\n";
        app.undo();
        return false;
    }
};

// 全局命令历史记录就是一个堆桟。
class CommandHistory {
    stack<unique_ptr<Command>> history;

   public:
    void push(unique_ptr<Command> command) { history.emplace(std::move(command)); }
    unique_ptr<Command> pop() {
        auto ptr = std::move(history.top());
        history.pop();
        return ptr;
    }
};

int main() {
    // App();
    Application app;
    app.createUI();
    return 0;
}

Application::Application(size_t n) {
    while (n-- > 0)
        editors.emplace_back(make_shared<Editor>());
    activeEditor = editors.front();
    history = make_unique<CommandHistory>();
}

void Application::createUI() {
    auto copy = [&]() { executeCommand(make_unique<CopyCommand>(*this, activeEditor)); };
    auto cut = [&]() { executeCommand(make_unique<CutCommand>(*this, activeEditor)); };
    auto paste = [&]() { executeCommand(make_unique<PasteCommand>(*this, activeEditor)); };
    auto undo = [&]() { executeCommand(make_unique<UndoCommand>(*this, activeEditor)); };
    copy();
    cut();
    paste();
    undo();
}

inline void Application::executeCommand(unique_ptr<Command> command) {
    if (command->execute())
        history->push(std::move(command));
}

inline void Application::undo() {
    if (auto command = history->pop(); command)
        command->undo();
}
