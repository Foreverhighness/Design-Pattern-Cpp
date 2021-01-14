#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// 原发器中包含了一些可能会随时间变化的重要数据。它还定义了在备忘录中保存
// 自身状态的方法，以及从备忘录中恢复状态的方法。
class Edtior {
 public:
  void setText(string_view text) { text_ = text; }
  void setCursor(const int x, const int y) { curX_ = x, curY_ = y; }
  void setSelectionWidth(const int width) { selectionWidth_ = width; }

  // 备忘录类保存有编辑器的过往状态。
  class Snapshot {
   public:
    Snapshot(Edtior& editor, string_view text, int curX, int curY, int selectionWidth)
        : editor_{editor}, text_{text}, curX_{curX}, curY_{curY}, selectionWidth_{selectionWidth} {}
    // 在某一时刻，编辑器之前的状态可以使用备忘录对象来恢复。
    void restore() const {
      editor_.setText(text_);
      editor_.setCursor(curX_, curY_);
      editor_.setSelectionWidth(selectionWidth_);
    }

   private:
    Edtior& editor_;
    string text_;
    int curX_, curY_, selectionWidth_;
  };
  // 在备忘录中保存当前的状态。
  unique_ptr<Snapshot> createSnapshot() {
    // 备忘录是不可变的对象；因此原发器会将自身状态作为参数传递给备忘
    // 录的构造函数。
    return make_unique<Snapshot>(*this, text_, curX_, curY_, selectionWidth_);
  }

  friend ostream& operator<<(ostream& os, const Edtior& obj) { return os << "text: " << obj.text_; }

 private:
  string text_{"init"};
  int curX_{}, curY_{}, selectionWidth_{};
};

// 命令对象可作为负责人。在这种情况下，命令会在修改原发器状态之前获取一个
// 备忘录。当需要撤销时，它会从备忘录中恢复原发器的状态。
class Command {
 public:
  void makeBackup(Edtior& editor) {
    backups.emplace_back(editor.createSnapshot());
    current = backups.size() - 1;
  }
  void undo() {
    if (current <= 0)
      return;
    --current;
    backups[current]->restore();
  }
  void redo() {
    if (current + 1 >= backups.size())
      return;
    ++current;
    backups[current]->restore();
  }

 private:
  vector<unique_ptr<Edtior::Snapshot>> backups;
  int current{0};
};

struct App {
  App() {
    Command cmd;
    Edtior e;
    cmd.makeBackup(e);
    e.setText("first");
    cmd.makeBackup(e);
    cout << e << endl;
    e.setText("second");
    cmd.makeBackup(e);
    cout << e << endl;
    cmd.undo();
    cout << "Undo2-> " << e << endl;
    cmd.undo();
    cout << "Undo1-> " << e << endl;
    cmd.redo();
    cout << "Redo1-> " << e << endl;
    cmd.redo();
    cout << "Redo2-> " << e << endl;
  }
};

int main() {
  App();
  return 0;
}
