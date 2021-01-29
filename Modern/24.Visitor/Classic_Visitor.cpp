#include <iostream>
#include <sstream>
using namespace std;

struct DoubleExpression;
struct AdditionExpression;
struct ExpressionVisitor {
  virtual ~ExpressionVisitor() = default;
  virtual void visit(DoubleExpression& de) = 0;
  virtual void visit(AdditionExpression& ae) = 0;
};

struct Expression {
  virtual ~Expression() = default;
  virtual void accept(ExpressionVisitor& visitor) = 0;
};

struct DoubleExpression : public Expression {
  double value;
  explicit DoubleExpression(const double value) : value{value} {}
  void accept(ExpressionVisitor& visitor) override { visitor.visit(*this); }
};
struct AdditionExpression : public Expression {
  Expression *left, *right;
  AdditionExpression(Expression* const left, Expression* const right) : left{left}, right{right} {}
  ~AdditionExpression() {
    delete left;
    delete right;
  }
  void accept(ExpressionVisitor& visitor) override { visitor.visit(*this); }
};

struct ExpressionPrinter : public ExpressionVisitor {
  void visit(DoubleExpression& de) override { oss_ << de.value; }
  void visit(AdditionExpression& ae) override {
    oss_ << "(";
    ae.left->accept(*this);
    oss_ << "+";
    ae.right->accept(*this);
    oss_ << ")";
  }
  string str() const { return oss_.str(); }

 private:
  ostringstream oss_;
};

struct ExpressionEvaluator : public ExpressionVisitor {
  int result;

  void visit(DoubleExpression& de) override { result = de.value; }
  void visit(AdditionExpression& ae) override {
    ae.left->accept(*this);
    auto temp = result;
    ae.right->accept(*this);
    result += temp;
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
  ExpressionPrinter printer;
  ExpressionEvaluator evaluator;
  printer.visit(e);
  evaluator.visit(e);
  cout << printer.str() << " = " << evaluator.result << endl;
}

int main() {
  test();
  return 0;
}
