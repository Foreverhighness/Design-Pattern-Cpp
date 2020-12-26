#include <iostream>
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

struct EventData {
  virtual ~EventData() = default;
  virtual void print() const = 0;
};

struct PlayerScoredData : public EventData {
  string player_name;
  int goals_scored_so_far;

  PlayerScoredData(string_view name, const int goal) : player_name{name}, goals_scored_so_far{goal} {}

  void print() const override { cout << player_name << " has scored! (their " << goals_scored_so_far << " goal)\n"; }
};

struct Game {
  signal<void(EventData*)> events;  // observer
};

struct Player {
  string name;
  Game& game;
  int goals_scored{0};

  Player(string_view name, Game& game) : name{name}, game{game} {}

  void score() {
    ++goals_scored;
    PlayerScoredData ps{name, goals_scored};
    game.events(&ps);
  }
};

struct Coach {
  Game& game;
  explicit Coach(Game& game) : game{game} {
    // celebrate if player has scored <3 goals
    game.events.connect([](EventData* e) {
      if (PlayerScoredData* ps = dynamic_cast<PlayerScoredData*>(e); ps && ps->goals_scored_so_far < 3)
        cout << "coach says: well done, " << ps->player_name << "\n";
    });
  }
};

void test() {
  Game game;
  Player player{"Sam", game};
  Coach coach{game};

  player.score();
  player.score();
  player.score();  // ignored by coach
}

int main() {
  test();
  return 0;
}