#include <iostream>
#include <sstream>
using namespace std;

struct Expression {
  virtual ~Expression() = default;
  virtual void print(ostringstream& oss) = 0;
};

struct DoubleExpression : public Expression {
  double value;
  explicit DoubleExpression(const double value) : value{value} {}
  void print(ostringstream& oss) override { oss << value; }
};
struct AdditionExpression : public Expression {
  Expression *left, *right;
  AdditionExpression(Expression* const left, Expression* const right) : left{left}, right{right} {}
  ~AdditionExpression() {
    delete left;
    delete right;
  }
  void print(ostringstream& oss) override {
    oss << "(";
    left->print(oss);
    oss << "+";
    right->print(oss);
    oss << ")";
  }
};

void test() {
  auto e = AdditionExpression{
      new DoubleExpression{1},
      new AdditionExpression{
          new DoubleExpression{2},
          new DoubleExpression{3},
      },
  };
  ostringstream oss;
  e.print(oss);
  cout << oss.str() << endl;  // prints (1+(2+3))
}

int main() {
  test();
  return 0;
}
