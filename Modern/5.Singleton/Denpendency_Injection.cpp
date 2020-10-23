#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <boost/di.hpp>
namespace di = boost::di;

class FakeSingleton {
    string name_;

   public:
    FakeSingleton(string_view s) : name_(s) { cout << "new instance here.\n"; }
    FakeSingleton(const FakeSingleton& other) : name_(other.name_) { cout << "copy instance here.\n"; }
    string name() const { return name_; }
};
// The user will use the same instance, but you can create more instances of FakeSingleton by yourself.
class UserOfSingleton {
    FakeSingleton& instance_;

   public:
    explicit UserOfSingleton(FakeSingleton& instance) : instance_(instance) {}
    string name() const { return instance_.name(); }
};

void test() {
    // clang-format off
    auto injector = di::make_injector(
        di::bind<FakeSingleton>().to<FakeSingleton>().in(di::singleton)
      , di::bind<string_view>().to("Singleton")
    );
    // clang-format on
    cout << "If you see the same value, then singleton was reused (yay!\n"
         << "If you see different values, then more singletons were created (booo!!)\n\n"
         << "RESULT:\n";
    vector<thread> threads;
    for (int i = 0; i < 5; ++i)
        threads.emplace_back([&]() {
            // Following code emulates slow initialization.
            this_thread::sleep_for(chrono::milliseconds(1000));
            auto user = injector.create<UserOfSingleton>();
            cout << user.name() << "\n";
        });
    for_each(threads.begin(), threads.end(), [](thread& t) { t.join(); });
}

int main() {
    test();
    return 0;
}