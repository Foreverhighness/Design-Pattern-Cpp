#include <array>
#include <iostream>
using namespace std;

enum class CreatureAbility { strength, agility, length };
class CreatureCommand {
 public:
  enum Action { set, increaseBy, decreaseBy } action;
  CreatureAbility ability;
  int amount;
};

class CreatureQuery {
 public:
  CreatureAbility ability;
};

class Creature {
 public:
  Creature(string_view name, int strength, int agility) : name{name}, arr_{strength, agility} {}

  void process_command(const CreatureCommand& cc) {
    size_t ability = static_cast<size_t>(cc.ability);
    switch (cc.action) {
      case CreatureCommand::set:
        arr_[ability] = cc.amount;
        break;
      case CreatureCommand::increaseBy:
        arr_[ability] += cc.amount;
        break;
      case CreatureCommand::decreaseBy:
        arr_[ability] -= cc.amount;
        break;
    }
  }
  int process_query(const CreatureQuery& q) const { return arr_[static_cast<size_t>(q.ability)]; }

  void set_strength(int value) {
    process_command(CreatureCommand{CreatureCommand::set, CreatureAbility::strength, value});
  }
  void set_agility(int value) {
    process_command(CreatureCommand{CreatureCommand::set, CreatureAbility::agility, value});
  }

  int get_strength() const { return process_query(CreatureQuery{CreatureAbility::strength}); }
  int get_agility() const { return process_query(CreatureQuery{CreatureAbility::agility}); }

  friend std::ostream& operator<<(std::ostream& os, const Creature& obj) {
    return os << "name: " << obj.name << "\n"
              << "str: " << obj.arr_[static_cast<size_t>(CreatureAbility::strength)] << "\n"
              << "agi " << obj.arr_[static_cast<size_t>(CreatureAbility::agility)];
  }

 private:
  string name;
  array<int, static_cast<size_t>(CreatureAbility::length)> arr_;
};

void test() {
  Creature goblin{"Goblin", 1, 1};
  cout << goblin << endl;
  goblin.set_strength(2);
  cout << "str: " << goblin.get_strength() << endl;
  cout << goblin << endl;
}

int main() {
  test();
  return 0;
}