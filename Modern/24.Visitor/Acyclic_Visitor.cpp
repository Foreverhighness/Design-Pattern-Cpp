#include <iostream>
#include <sstream>
using namespace std;

template <typename Visitable>
struct Visitor {
  virtual ~Visitor() = default;
  virtual void visit(Visitable& obj) = 0;
};

struct VisitorBase {  // marker trait
  virtual ~VisitorBase() = default;
};

struct Expression {
  virtual ~Expression() = default;
  virtual void accept(VisitorBase& visitor) {
    using EV = Visitor<std::remove_reference_t<decltype(*this)>>;
    if (auto e = dynamic_cast<EV*>(&visitor))
      e->visit(*this);
  }
};

struct DoubleExpression : public Expression {
  double value;
  explicit DoubleExpression(const double value) : value{value} {}
  void accept(VisitorBase& visitor) override {
    using EV = Visitor<std::remove_reference_t<decltype(*this)>>;
    if (auto e = dynamic_cast<EV*>(&visitor))
      e->visit(*this);
  }
};
struct AdditionExpression : public Expression {
  Expression *left, *right;
  AdditionExpression(Expression* const left, Expression* const right) : left{left}, right{right} {}
  ~AdditionExpression() {
    delete left;
    delete right;
  }
  virtual void accept(VisitorBase& visitor) override {
    using EV = Visitor<std::remove_reference_t<decltype(*this)>>;
    if (auto e = dynamic_cast<EV*>(&visitor))
      e->visit(*this);
  }
};

struct ExpressionPrinter : public VisitorBase,
                           public Visitor<Expression>,
                           public Visitor<DoubleExpression>,
                           public Visitor<AdditionExpression> {
  void visit(Expression& e) override {}
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

struct ExpressionEvaluator : public VisitorBase,
                             public Visitor<Expression>,
                             public Visitor<DoubleExpression>,
                             public Visitor<AdditionExpression> {
  void visit(Expression& e) override {}
  void visit(DoubleExpression& de) override { result = de.value; }
  void visit(AdditionExpression& ae) override {
    ae.left->accept(*this);
    auto temp = result;
    ae.right->accept(*this);
    result += temp;
  }

  int result;
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
