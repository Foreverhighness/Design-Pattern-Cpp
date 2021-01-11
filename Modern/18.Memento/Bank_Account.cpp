#include <iostream>
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
  explicit BankAccount(const int balance) : balance_{balance} {}

  Memento deposit(const int amount) {
    balance_ += amount;
    return {balance_};
  }
  void restore(const Memento& m) { balance_ = m.balance_; }

  friend ostream& operator<<(ostream& os, const BankAccount& obj) { return os << "Balance: " << obj.balance_; }

 private:
  int balance_{0};
};

void test() {
  BankAccount ba{100};
  auto m1 = ba.deposit(50);
  auto m2 = ba.deposit(25);
  cout << ba << "\n";  // Balance: 175
  // undo to m1
  ba.restore(m1);
  cout << ba << "\n";  // Balance: 150
  // redo
  ba.restore(m2);
  cout << ba << "\n";  // Balance: 175
}

int main() {
  test();
  return 0;
}
