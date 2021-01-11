#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Memento {
 public:
  Memento(const int balance) : balance_{balance} {}

  friend class BankAccount;

 private:
  int balance_;
};

class BankAccount {
 public:
  explicit BankAccount(const int balance) : balance_{balance} {
    changes_.emplace_back(make_shared<Memento>(balance));
    current_ = 0;
  }

  shared_ptr<Memento> deposit(const int amount) {
    balance_ += amount;
    const auto& m = make_shared<Memento>(balance_);
    changes_.emplace_back(m);
    current_ = changes_.size() - 1;
    return m;
  }
  void restore(const shared_ptr<Memento>& m) {
    if (m) {
      balance_ = m->balance_;
      changes_.emplace_back(m);
      current_ = changes_.size() - 1;
    }
  }

  shared_ptr<Memento> undo() {
    if (current_ <= 0)
      return nullptr;

    --current_;
    const auto& m = changes_[current_];
    balance_ = m->balance_;
    return m;
  }
  shared_ptr<Memento> redo() {
    if (current_ + 1 >= changes_.size())
      return nullptr;

    ++current_;
    const auto& m = changes_[current_];
    balance_ = m->balance_;
    return m;
  }

  friend ostream& operator<<(ostream& os, const BankAccount& obj) { return os << "Balance: " << obj.balance_; }

 private:
  vector<shared_ptr<Memento>> changes_;
  int balance_{0};
  int current_{0};
};

void test() {
  BankAccount ba{100};
  ba.deposit(50);
  ba.deposit(25);
  cout << ba << "\n";  // Banlance: 175
  ba.undo();
  cout << "Undo 1: " << ba << "\n";  // Undo 1: Banlance: 150
  ba.undo();
  cout << "Undo 2: " << ba << "\n";  // Undo 2: Banlance: 100
  ba.redo();
  cout << "Redo 2: " << ba << "\n";  // Redo 2: Banlance: 150
  ba.undo();                         // back to 100 again
  cout << "Redo 1: " << ba << "\n";  // Redo 1: Banlance: 100
}

int main() {
  test();
  return 0;
}
