#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
using namespace std;

class Player {
    // strength, agility, intelligence
    enum Abilities { Str, Agi, Int, Len };
    array<int, Len> arr_;
   public:
    int get_strength() const { return arr_[Str]; }
    void set_strength(int strength) { arr_[Str] = strength; }

    int get_agility() const { return arr_[Agi]; }
    void set_agility(int agility) { arr_[Agi] = agility; }

    int get_intelligence() const { return arr_[Int]; }
    void set_intelligence(int intelligence) { arr_[Int] = intelligence; }
    __declspec(property(get = get_strength, put = set_strength)) int strength;
    __declspec(property(get = get_agility, put = set_agility)) int agility;
    __declspec(property(get = get_intelligence, put = set_intelligence)) int intelligence;
    int sum() const { return accumulate(begin(arr_), end(arr_), 0); }

    double average() const { return sum() / static_cast<double>(Len); }

    int max() const { return *max_element(begin(arr_), end(arr_)); }
};

void test() {
    Player orc;
    orc.strength = 16;
    orc.agility = 11;
    orc.intelligence = 9;

    cout << "The orc has an average stat of " << orc.average() << " and a maximum stat of " << orc.max() << "\n";
}
int main() {
    test();
    return 0;
}