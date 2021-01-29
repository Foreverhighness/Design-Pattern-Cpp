#include <iostream>
#include <sstream>
using namespace std;

struct Expression {
  virtual ~Expression() = default;
};

struct DoubleExpression : public Expression {
  double value;
  explicit DoubleExpression(const double value) : value{value} {}
};
struct AdditionExpression : public Expression {
  Expression *left, *right;
  AdditionExpression(Expression* const left, Expression* const right) : left{left}, right{right} {}
  ~AdditionExpression() {
    delete left;
    delete right;
  }
};

struct ExpressionPrinter {
  void print(Expression* e) {
    if (auto de = dynamic_cast<DoubleExpression*>(e)) {
      oss_ << de->value;
    } else if (auto ae = dynamic_cast<AdditionExpression*>(e)) {
      oss_ << "(";
      print(ae->left);
      oss_ << "+";
      print(ae->right);
      oss_ << ")";
    }
  }
  string str() const { return oss_.str(); }

 private:
  ostringstream oss_;
};

void test() {
  auto e = AdditionExpression{
      new DoubleExpression{1},
      new AdditionExpression{
          new DoubleExpression{2},
          new DoubleExpression{3},
      },
  };
  ExpressionPrinter ep;
  ep.print(&e);
  cout << ep.str() << endl;
}

int main() {
  test();
  return 0;
}
