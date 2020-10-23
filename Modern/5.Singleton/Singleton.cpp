#include <algorithm>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;
class FakeSingleton {
    unordered_map<string, int> data;
    string name_;
    FakeSingleton(string_view s) : name_(s) { data["Tokyo"] = 123; }

   public:
    static FakeSingleton& instance(string_view s) {
        static FakeSingleton _i(s);
        return _i;
    }
    int get(const string& key) {
        if (auto it = data.find(key); it != data.end())
            return it->second;
        data[key] = 0;
        return 0;
    }
    string name() const { return name_; }

    // ~Singleton() = delete;
    FakeSingleton(FakeSingleton&) = delete;
    FakeSingleton(FakeSingleton&&) = delete;
    void operator=(FakeSingleton&) = delete;
};

int main() {
    cout << "If you see the same value, then singleton was reused (yay!\n"
         << "If you see different values, then more singletons were created (booo!!)\n\n"
         << "RESULT:\n";
    vector<string> paras{"Foo", "Bar", "Baz", "Circle", "Point"};
    vector<thread> threads;
    for_each(paras.begin(), paras.end(), [&threads](string_view s) {
        threads.emplace_back([s]() {
            // Following code emulates slow initialization.
            this_thread::sleep_for(chrono::milliseconds(1000));
            FakeSingleton& singleton = FakeSingleton::instance(s);
            cout << singleton.name() << "\n";
        });
    });
    for_each(threads.begin(), threads.end(), [](thread& t) { t.join(); });
    return 0;
}