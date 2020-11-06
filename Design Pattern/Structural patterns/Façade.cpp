#include <iostream>
#include <memory>
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
    shared_ptr<Subsystem1> subsystem1_;
    shared_ptr<Subsystem2> subsystem2_;

   public:
    Façade(const shared_ptr<Subsystem1>& subsystem1 = nullptr, const shared_ptr<Subsystem2>& subsystem2 = nullptr) {
        subsystem1_ = subsystem1 ?: make_shared<Subsystem1>();
        subsystem2_ = subsystem2 ?: make_shared<Subsystem2>();
    }
    string Operation() {
        string result = "Facade initializes subsystems:\n";
        result += subsystem1_->Operation1();
        result += subsystem2_->Operation1();
        result += "Facade orders subsystems to perform the action:\n";
        result += subsystem1_->Operation2();
        result += subsystem2_->Operation2();
        return result;
    }
};

struct App {
    App() {
        auto f = make_unique<Façade>(make_shared<Subsystem1>());
        cout << f->Operation();
    }
};

int main(int argc, char const* argv[]) {
    App();
    return 0;
}
