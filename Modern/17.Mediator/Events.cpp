#include <iostream>
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

struct EventData {
  virtual ~EventData() = default;
  virtual void print() const = 0;
};

class PlayerScoredData : public EventData {
 public:
  string player_name;
  int goals_scored_so_far;

  PlayerScoredData(string_view name, const int goal) : player_name{name}, goals_scored_so_far{goal} {}
  void print() const override { cout << player_name << " has scored! (their " << goals_scored_so_far << " goal)\n"; }
};

struct Game {
  signal<void(EventData*)> events;  // observer
};

class Player {
 public:
  Player(string_view name, Game& game) : name{name}, game{game} {}
  void score() {
    ++goals_scored;
    PlayerScoredData ps{name, goals_scored};
    game.events(&ps);
  }

 private:
  string name;
  Game& game;
  int goals_scored{0};
};

class Coach {
 public:
  explicit Coach(Game& game) : game{game} {
    // celebrate if player has scored <3 goals
    game.events.connect([](EventData* e) {
      if (auto ps = dynamic_cast<PlayerScoredData*>(e); ps && ps->goals_scored_so_far < 3)
        cout << "coach says: well done, " << ps->player_name << "\n";
    });
  }

 private:
  Game& game;
};

void test() {
  Game game;
  auto sam = make_shared<Player>("Sam", game);
  auto coach = make_unique<Coach>(game);

  sam->score();
  sam->score();
  sam->score();  // ignored by coach
}

int main() {
  test();
  return 0;
}