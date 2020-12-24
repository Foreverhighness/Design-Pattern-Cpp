#include <iostream>
using namespace std;

class BankAccount {
 public:
  int balance{0};
  int overdraft_limit{-500};

  void deposit(int amount) {
    balance += amount;
    cout << "deposited " << amount << ", balance is now " << balance << "\n";
  }

  void withdraw(int amount) {
    if (balance - amount >= overdraft_limit) {
      balance -= amount;
      cout << "withdrew " << amount << ", balance is now " << balance << "\n";
    }
  }
};

struct Command {
  virtual ~Command() = default;
  virtual void call() const = 0;
};

class BankAccountCommand : public Command {
 public:
  BankAccount& account;
  enum class Action { deposit, withdraw } action;
  int amount;
  BankAccountCommand(BankAccount& account, const Action action, const int amount)
      : account{account}, action{action}, amount{amount} {}
  void call() const override {
    switch (action) {
      case Action::deposit:
        account.deposit(amount);
        break;
      case Action::withdraw:
        account.withdraw(amount);
        break;
    }
  }
};

void test() {
  BankAccount ba;
  const Command& cmd = BankAccountCommand{ba, BankAccountCommand::Action::deposit, 100};
  cmd.call();
}

int main() {
  test();
  return 0;
}
