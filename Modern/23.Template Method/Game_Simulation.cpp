#include <iostream>
using namespace std;

class Game {
 public:
  explicit Game(const int number_of_players) : number_of_players_{number_of_players} {}
  void run() {
    start();
    while (!have_winner())
      take_turn();
    cout << "Player " << get_winner() << " wins.\n";
  }

 protected:
  virtual void start() = 0;
  virtual bool have_winner() = 0;
  virtual void take_turn() = 0;
  virtual int get_winner() = 0;

  int number_of_players_;
  int current_player_{0};
};

class Chess : public Game {
 public:
  explicit Chess() : Game{2} {}

 protected:
  void start() override { cout << "Starting a game of chess with " << number_of_players_ << " players\n"; }
  bool have_winner() override { return turns_ == max_turns_; }
  void take_turn() override {
    cout << "Turn " << turns_ << " taken by player " << current_player_ << "\n";
    turns_++;
    current_player_ = (current_player_ + 1) % number_of_players_;
  }
  int get_winner() override { return current_player_; }

 private:
  int turns_{0}, max_turns_{10};
};

void test() {
  Chess chess;
  Game& game = chess;
  game.run();
}

int main() {
  test();
  return 0;
}
