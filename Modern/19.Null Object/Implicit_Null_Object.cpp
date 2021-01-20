#include <iostream>
#include <memory>
using namespace std;

#include <boost/lexical_cast.hpp>

struct Logger {
  virtual ~Logger() = default;
  virtual void info(string_view s) = 0;
  virtual void warn(string_view s) = 0;
};

// double-hop
class OptionalLogger : public Logger {
 public:
  // inline static shared_ptr<Logger> no_logging;
  OptionalLogger(const shared_ptr<Logger>& logger) : impl_{logger} {}

  void info(string_view s) override {
    if (impl_)
      impl_->info(s);
  }
  void warn(string_view s) override {
    if (impl_)
      impl_->warn(s);
  }

 private:
  shared_ptr<Logger> impl_;
};

class BankAccount {
 public:
  BankAccount(string_view name, const int balance, const shared_ptr<Logger>& logger = nullptr) // = OptionalLogger::no_logging)
      : logger_{make_shared<OptionalLogger>(logger)}, name_{name}, balance_{balance} {}

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

void test() {
  {
    shared_ptr<Logger> logger{make_shared<ConsoleLogger>()};
    BankAccount account{"primary account", 1000, logger};
    account.deposit(2000);
  }
  {
    BankAccount account{"secondary account", 1000};
    account.deposit(2000);
  }
}

int main() {
  test();
  return 0;
}
