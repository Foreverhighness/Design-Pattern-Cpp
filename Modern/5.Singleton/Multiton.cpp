#include <iostream>
#include <map>
#include <memory>
#include <cassert>
using namespace std;
enum class Importance {
    PRIMARY,
    SECONDARY,
    TERTIARY,
};
class Printer {
   public:
    Printer() { ++ctor_cnt(); }
    static int& ctor_cnt() {
        static int cnt = 0;
        return cnt;
    }
};
template <typename Value, typename Key = string>
class Multiton {
   public:
    static shared_ptr<Value> get(const Key& key) {
        static map<Key, shared_ptr<Value>> map_;
        if (const auto it = map_.find(key); it != map_.end())
            return it->second;

        auto instance = make_shared<Value>();
        map_[key] = instance;
        return instance;
    }
};
void test() {
    using mt = Multiton<Printer, Importance>;
    mt::get(Importance::PRIMARY);
    assert(1 == Printer::ctor_cnt());
    mt::get(Importance::PRIMARY);
    assert(1 == Printer::ctor_cnt());
    mt::get(Importance::SECONDARY);
    assert(2 == Printer::ctor_cnt());
    mt::get(Importance::SECONDARY);
    assert(2 == Printer::ctor_cnt());
    mt::get(Importance::TERTIARY);
    assert(3 == Printer::ctor_cnt());
    mt::get(Importance::TERTIARY);
    assert(3 == Printer::ctor_cnt());
}
int main() {
    test();
    return 0;
}