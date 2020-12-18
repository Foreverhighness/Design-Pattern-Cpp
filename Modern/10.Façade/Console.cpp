#include <iostream>
#include <memory>
#include <optional>
using namespace std;

/**
 * The Subsystem can accept requests either from the facade or client directly.
 * In any case, to the Subsystem, the Facade is yet another client, and it's not
 * a part of the Subsystem.
 */
class Subsystem1 {
 public:
  string Operation1() const { return "Subsystem1: Ready to go!\n"; }
  string Operation2() const { return "Subsystem1: Go!\n"; }
};
class Subsystem2 {
 public:
  string Operation1() const { return "Subsystem2: Ready!\n"; }
  string Operation2() const { return "Subsystem2: Fire!\n"; }
};

class Façade {
 protected:
  /**
   * Depending on your application's needs, you can provide the Facade with
   * existing subsystem objects or force the Facade to create them on its own.
   */
  Subsystem1 subsystem1_;
  Subsystem2 subsystem2_;

 public:
  Façade(optional<Subsystem1> subsystem1, optional<Subsystem2> subsystem2) {
    subsystem1_ = subsystem1.value_or(Subsystem1());
    subsystem2_ = subsystem2.value_or(Subsystem2());
  }
  string Operation() {
    string result = "Facade initializes subsystems:\n";
    result += subsystem1_.Operation1();
    result += subsystem2_.Operation1();
    result += "Facade orders subsystems to perform the action:\n";
    result += subsystem1_.Operation2();
    result += subsystem2_.Operation2();
    return result;
  }
};

struct App {
  App() {
    auto f = make_unique<Façade>(nullopt, Subsystem1());
    cout << f->Operation();
  }
};

int main(int argc, char const* argv[]) {
  App();
  return 0;
}
