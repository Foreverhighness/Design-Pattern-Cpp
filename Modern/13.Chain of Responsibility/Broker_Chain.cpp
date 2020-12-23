#include <iostream>
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

class Query {
 public:
  string creature_name;
  enum class Argument { attack, defense } argument;
  int result;
};

class Game {
 public:
  signal<void(Query&)> queries;
};

class Creature {
 public:
  string name;
  Creature(Game& game, string_view name, const int attack, const int defense)
      : name{name}, game_{game}, attack_{attack}, defense_{defense} {}

  int get_attack() const {
    Query q{name, Query::Argument::attack, attack_};
    game_.queries(q);
    return q.result;
  }
  int get_defense() const {
    Query q{name, Query::Argument::defense, defense_};
    game_.queries(q);
    return q.result;
  }
  friend ostream& operator<<(ostream& os, const Creature& obj) {
    return os << "name: " << obj.name << " attack: " << obj.get_attack() << " defense: " << obj.get_defense();
  }

 private:
  Game& game_;
  int attack_, defense_;
};

class CreatureModifier {
 public:
  virtual ~CreatureModifier() {}
  CreatureModifier(Game& game, Creature& creature) : game_{game}, creature_{creature} {}

 protected:
  Game& game_;
  Creature creature_;
};

class DoubleAttackModifier : public CreatureModifier {
 public:
  DoubleAttackModifier(Game& game, Creature& creature) : CreatureModifier{game, creature} {
    conn_ = game_.queries.connect([&](Query& q) {
      if (q.creature_name == creature_.name && q.argument == Query::Argument::attack)
        q.result *= 2;
    });
  }
  ~DoubleAttackModifier() { conn_.disconnect(); }

 private:
  connection conn_;
};

void test() {
  Game game;
  Creature goblin{game, "Strong Goblin", 2, 2};
  cout << goblin << endl;  //.. name: Strong Goblin attack: 2 defense: 2
  {
    DoubleAttackModifier dam{game, goblin};
    cout << goblin << endl;  // name: Strong Goblin attack: 4 defense: 2
  }
  cout << goblin << endl;  //.. name: Strong Goblin attack: 2 defense: 2
}

int main() {
  test();
  return 0;
}