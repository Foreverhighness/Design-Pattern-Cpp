#include <iostream>
#include <memory>
using namespace std;

#include <boost/lexical_cast.hpp>

struct Logger {
  virtual ~Logger() = default;
  virtual void info(string_view s) = 0;
  virtual void warn(string_view s) = 0;
};

class BankAccount {
 public:
  BankAccount(const shared_ptr<Logger>& logger, string_view name, const int balance)
      : logger_{logger}, name_{name}, balance_{balance} {}

  void deposit(const int amount) {
    using boost::lexical_cast;
    balance_ += amount;
    logger_->info("Deposited $"s + lexical_cast<string>(amount) + " to "s + name_ + ", balance is now $"s +
                  lexical_cast<string>(balance_));
  }

 private:
  shared_ptr<Logger> logger_;
  string name_;
  int balance_{0};
};

class ConsoleLogger : public Logger {
 public:
  void info(string_view s) override { cout << "INFO: " << s << endl; }
  void warn(string_view s) override { cout << "WARN: " << s << endl; }
};

class NullLogger : public Logger {
 public:
  void info(string_view s) override {}
  void warn(string_view s) override {}
};

void test() {
  {
    shared_ptr<Logger> logger{make_shared<ConsoleLogger>()};
    BankAccount account{logger, "primary account", 1000};
    account.deposit(2000);
  }
  {
    shared_ptr<Logger> logger{make_shared<NullLogger>()};
    BankAccount account{logger, "secondary account", 1000};
    account.deposit(2000);
  }
}

int main() {
  test();
  return 0;
}
