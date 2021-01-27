#include <iostream>
#include <vector>
using namespace std;

class Creature {
 public:
  Creature(const int x, const int y) : position{x, y} {}
  pair<int, int> position;
};

// 抽象类定义了一个模板方法，其中通常会包含某个由抽象原语操作调用组成的算
// 法框架。具体子类会实现这些操作，但是不会对模板方法做出修改。
class GameAI {
 public:
  // 模板方法定义了某个算法的框架。
  void turn() {
    collectResources();
    buildStructures();
    buildUnits();
    attack();
  }

 protected:
  // 某些步骤可在基类中直接实现。
  virtual void collectResources() {
    for (auto&& s : buildStructures())
      cout << "ready to build " << s << endl;
  }
  virtual vector<string>& buildStructures() = 0;
  virtual void buildUnits() = 0;
  // 一个类可包含多个模板方法。
  virtual void attack() {
    auto enemy = closestEnemy();
    if (enemy)
      sendWarriors(enemy->position);
    else
      sendScouts("center");
  }

 private:
  Creature* closestEnemy() const {
    static Creature enemy{100, 200};
    return &enemy;
  }
  void sendScouts(string_view position) { cout << "go to map center\n"; }
  void sendWarriors(pair<int, int> position) {
    auto [x, y] = position;
    cout << "go to (" << x << ", " << y << ")\n";
  }
};

// 具体类必须实现基类中的所有抽象操作，但是它们不能重写模板方法自身。
class OrcsAI : public GameAI {
 protected:
  vector<string>& buildStructures() override {
    static vector<string> structures{"farm"s, "castle"s, "stronghold"s};
    return structures;
  }
  void buildUnits() override { cout << "Build peon, add it to scouts group.\n"; }
};
class MonstersAI : public GameAI {
 protected:
  void collectResources() override {}
  vector<string>& buildStructures() override {
    static vector<string> structures{};
    return structures;
  }
  void buildUnits() override {}
};

struct App {
  App() {
    {
      OrcsAI orc;
      GameAI& ai = orc;
      ai.turn();
    }
    cout << "--------------------------------\n";
    {
      MonstersAI monster;
      GameAI& ai = monster;
      ai.turn();
    }
  }
};

int main() {
  App();
  return 0;
}
