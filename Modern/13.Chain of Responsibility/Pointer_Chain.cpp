#include <iostream>
using namespace std;

class Creature {
 public:
  string name;
  int attack, defense;
  Creature(string_view name, const int attack, const int defense) : name{name}, attack{attack}, defense{defense} {}

  friend ostream& operator<<(ostream& os, const Creature& obj) {
    return os << "name: " << obj.name << " attack: " << obj.attack << " defense: " << obj.defense;
  }
};

class CreatureModifier {
 public:
  virtual ~CreatureModifier() = default;
  explicit CreatureModifier(Creature& creature) : creature{creature} {}

  void add(CreatureModifier* cm) {
    if (next_)
      next_->add(cm);
    else
      next_ = cm;
  }

  virtual void handle() {
    if (next_)
      next_->handle();
  }

 private:
  CreatureModifier* next_{nullptr};

 protected:
  Creature& creature;
};

class DoubleAttackModifier : public CreatureModifier {
 public:
  explicit DoubleAttackModifier(Creature& creature) : CreatureModifier{creature} {}

  void handle() override {
    creature.attack *= 2;
    CreatureModifier::handle();
  }
};

class IncreaseDefenseModifier : public CreatureModifier {
 public:
  explicit IncreaseDefenseModifier(Creature& creature) : CreatureModifier{creature} {}

  void handle() override {
    if (creature.attack <= 2)
      creature.defense += 1;
    CreatureModifier::handle();
  }
};

class NoBonusesModifier : public CreatureModifier {
 public:
  explicit NoBonusesModifier(Creature& creature) : CreatureModifier{creature} {}

  void handle() override {
    // nothing
  }
};
void test() {
  Creature goblin{"Goblin", 1, 1};
  CreatureModifier root{goblin};
  DoubleAttackModifier r1{goblin};
  DoubleAttackModifier r1_2{goblin};
  IncreaseDefenseModifier r2{goblin};
  // NoBonusesModifier nb{ goblin }; // effectively Command objects

  // root.add(&nb);
  root.add(&r1);
  root.add(&r1_2);
  root.add(&r2);

  root.handle();  // annoying

  cout << goblin << endl;
}

int main() {
  test();
  return 0;
}