#include <iostream>
#include <memory>
using namespace std;

// 策略接口声明了某个算法各个不同版本间所共有的操作。上下文会使用该接口来
// 调用有具体策略定义的算法。
struct Strategy {
  virtual ~Strategy() = default;
  virtual int execute(const int a, const int b) = 0;
};

// 具体策略会在遵循策略基础接口的情况下实现算法。该接口实现了它们在上下文
// 中的互换性。
struct ConcreteStrategyAdd : Strategy {
  int execute(const int a, const int b) override { return a + b; }
};
struct ConcreteStrategySubtract : Strategy {
  int execute(const int a, const int b) override { return a - b; }
};
struct ConcreteStrategyMultiply : Strategy {
  int execute(const int a, const int b) override { return a * b; }
};

// 上下文定义了客户端关注的接口。
class Context {
 public:
  // 上下文通常会通过构造函数来接收策略对象，同时还提供设置器以便在运行
  // 时切换策略。
  void setStrategy(unique_ptr<Strategy>&& strategy) { strategy_ = std::move(strategy); }
  // 上下文会将一些工作委派给策略对象，而不是自行实现不同版本的算法。
  int executeStrategy(const int a, const int b) { return strategy_->execute(a, b); }

 private:
  // 上下文会维护指向某个策略对象的引用。上下文不知晓策略的具体类。上下
  // 文必须通过策略接口来与所有策略进行交互。
  unique_ptr<Strategy> strategy_;
};

enum class StrategyType {
  Add,
  Subtract,
  Multiply,
};

struct App {
  App() {
    Context context;
    int a = 100, b = 200;
    // cin >> a >> b;

    int action = 0;
    // cin >> action;
    switch (static_cast<StrategyType>(action)) {
      case StrategyType::Add:
        context.setStrategy(make_unique<ConcreteStrategyAdd>());
        break;
      case StrategyType::Subtract:
        context.setStrategy(make_unique<ConcreteStrategySubtract>());
        break;
      case StrategyType::Multiply:
        context.setStrategy(make_unique<ConcreteStrategyMultiply>());
        break;
      default:
        throw "Unsupport Strategy.";
    }
    int result = context.executeStrategy(a, b);
    cout << result << endl;
  }
};

int main() {
  App();
  return 0;
}
