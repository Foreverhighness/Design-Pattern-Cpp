#include <iostream>
#include <vector>
using namespace std;

class BankAccount {
 public:
  int balance{0};
  int overdraft_limit{-500};

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
  bool succeeded{false};
  virtual ~Command() = default;
  virtual void call() = 0;
  virtual void undo() = 0;
};

class BankAccountCommand : public Command {
 public:
  BankAccount& account;
  enum class Action { deposit, withdraw } action;
  int amount;
  BankAccountCommand(BankAccount& account, const Action action, const int amount)
      : account{account}, action{action}, amount{amount} {}

  void call() override {
    switch (action) {
      case Action::deposit:
        account.deposit(amount);
        succeeded = true;
        break;
      case Action::withdraw:
        succeeded = account.withdraw(amount);
        break;
    }
  }
  void undo() override {
    if (!succeeded)
      return;

    switch (action) {
      case Action::deposit:
        account.withdraw(amount);
        break;
      case Action::withdraw:
        account.deposit(amount);
        break;
    }
  }
};

class CompositeBankAccountCommand : public vector<BankAccountCommand>, public Command {
 public:
  explicit CompositeBankAccountCommand(const initializer_list<value_type>& items) : vector<value_type>{items} {}

  void call() override {
    for (auto& cmd : *this)
      cmd.call();
  }
  void undo() override {
    for (auto it{this->rbegin()}; it != this->rend(); ++it)
      it->undo();
  }
};

class MoneyTransferCommand : public CompositeBankAccountCommand {
 public:
  MoneyTransferCommand(BankAccount& from, BankAccount& to, int amount)
      : CompositeBankAccountCommand({
            BankAccountCommand{from, BankAccountCommand::Action::withdraw, amount},
            BankAccountCommand{to, BankAccountCommand::Action::deposit, amount},
        }) {}
};

class DependentCompositeCommand : public CompositeBankAccountCommand {
 public:
  explicit DependentCompositeCommand(const initializer_list<value_type>& items) : CompositeBankAccountCommand{items} {}

  void call() override {
    for (auto it{this->begin()}; it != this->end(); ++it) {
      it->call();
      if (!it->succeeded) {
        for (reverse_iterator rit = reverse_iterator{it}; rit != this->rend(); ++rit)
          rit->undo();
        break;
      }
    }
  }
};

void test() {
  BankAccount ba1, ba2;
  ba1.deposit(100);
  Command&& composit_cmd{DependentCompositeCommand{
      BankAccountCommand{ba1, BankAccountCommand::Action::withdraw, 100},
      BankAccountCommand{ba2, BankAccountCommand::Action::deposit, 100},
  }};
  composit_cmd.call();
  cout << ba1.balance << "\n";
  cout << ba2.balance << "\n";

  Command&& cmd{DependentCompositeCommand{
      BankAccountCommand{ba1, BankAccountCommand::Action::deposit, 1000},
      BankAccountCommand{ba2, BankAccountCommand::Action::withdraw, 1000},
  }};
  cmd.call();
  cout << ba1.balance << "\n";
  cout << ba2.balance << "\n";
}

int main() {
  test();
  return 0;
}
