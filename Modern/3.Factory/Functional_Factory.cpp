#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
using namespace std;

struct HotDrink {
    virtual ~HotDrink() = default;
    virtual void perpare(int volume) = 0;
};
struct Tea : public HotDrink { void perpare(int volume) override { cout << "Take tea with " << volume << "ml." << endl; } };
struct Coffee : public HotDrink { void perpare(int volume) override { cout << "Take coffee with " << volume << "ml." << endl; } };

struct HotDrinkFactory {
    virtual ~HotDrinkFactory() = default;
    virtual unique_ptr<HotDrink> make() const = 0;
};

struct TeaFactory : public HotDrinkFactory { unique_ptr<HotDrink> make() const override { return make_unique<Tea>(); } };
struct CoffeeFactory : public HotDrinkFactory { unique_ptr<HotDrink> make() const override { return make_unique<Coffee>(); } };

class DrinkFactoryFactory {
    unordered_map<string, unique_ptr<HotDrinkFactory>> hot_factories;

   public:
    DrinkFactoryFactory() {
        hot_factories["coffee"] = make_unique<CoffeeFactory>();
        hot_factories["tea"] = make_unique<TeaFactory>();
    }
    unique_ptr<HotDrink> make_drink(const string& name) {
        auto drink = hot_factories[name]->make();
        drink->perpare(200);
        return drink;
    }
};

class DrinkWithVolumeFactoryFactory {
    unordered_map<string, function<unique_ptr<HotDrink>(int)>> hot_factories;

    public:
    DrinkWithVolumeFactoryFactory() {
        hot_factories["coffee"] = [](int x) {
            auto drink = CoffeeFactory().make();
            drink->perpare(x);
            return drink;
        };
        hot_factories["tea"] = [](int x) {
            auto drink = TeaFactory().make();
            drink->perpare(x);
            return drink;
        };
    }
    unique_ptr<HotDrink> make_drink(const string& name) {
        return hot_factories[name](200);
    }
};

void test() {
    DrinkFactoryFactory f;
    f.make_drink("tea");
    f.make_drink("coffee");
    DrinkWithVolumeFactoryFactory dwvff;
    dwvff.make_drink("tea");
    dwvff.make_drink("coffee");
}
int main() {
    test();
    return 0;
}