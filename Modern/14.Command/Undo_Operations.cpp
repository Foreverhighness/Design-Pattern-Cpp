#include <iostream>
using namespace std;

class BankAccount {
 public:
  int balance{0};
  int overdraft_limit{-50};

  void deposit(int amount) {
    balance += amount;
    cout << "deposited " << amount << ", balance is now " << balance << "\n";
  }

  bool withdraw(int amount) {
    if (balance - amount >= overdraft_limit) {
      balance -= amount;
      cout << "withdrew " << amount << ", balance is now " << balance << "\n";
      return true;
    }
    return false;
  }
};

struct Command {
  virtual ~Command() = default;
  virtual void call() const = 0;
  virtual void undo() const = 0;
};

class BankAccountCommand : public Command {
 public:
  BankAccount& account;
  enum class Action { deposit, withdraw } action;
  int amount;
  mutable bool withdrawal_succeeded{false};
  BankAccountCommand(BankAccount& account, const Action action, const int amount)
      : account{account}, action{action}, amount{amount} {}

  void call() const override {
    switch (action) {
      case Action::deposit:
        account.deposit(amount);
        break;
      case Action::withdraw:
        withdrawal_succeeded = account.withdraw(amount);
        break;
    }
  }
  void undo() const override {
    switch (action) {
      case Action::deposit:
        account.withdraw(amount);
        break;
      case Action::withdraw:
        if (withdrawal_succeeded)
          account.deposit(amount);
        break;
    }
  }
};

void test() {
  BankAccount ba;
  Command&& cmd_d = BankAccountCommand{ba, BankAccountCommand::Action::deposit, 100};
  Command&& cmd_w = BankAccountCommand{ba, BankAccountCommand::Action::withdraw, 200};
  cmd_d.call();
  cmd_w.call();
}

int main() {
  test();
  return 0;
}
