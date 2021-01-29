#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Dot;
class Circle;
class Rectangle;
class CompoundShape;
// 访问者接口声明了一组与元素类对应的访问方法。访问方法的签名能让访问者准
// 确辨别出与其交互的元素所属的类。
struct Visitor {
  virtual ~Visitor() = default;
  virtual void visit(Dot&) = 0;
  virtual void visit(Circle&) = 0;
  virtual void visit(Rectangle&) = 0;
  virtual void visit(CompoundShape&) = 0;
};

// 元素接口声明了一个`accept（接收）`方法，它会将访问者基础接口作为一个参数。
struct Shape {
  virtual ~Shape() = default;
  virtual void accept(Visitor& v) = 0;
};

// 每个具体元素类都必须以特定方式实现`accept`方法，使其能调用相应元素类的
// 访问者方法。
class Dot : public Shape {
 public:
  void accept(Visitor& visitor) override { visitor.visit(*this); }
};
class Circle : public Shape {
 public:
  void accept(Visitor& visitor) override { visitor.visit(*this); }
};
class Rectangle : public Shape {
 public:
  void accept(Visitor& visitor) override { visitor.visit(*this); }
};
class CompoundShape : public Shape {
 public:
  void accept(Visitor& visitor) override { visitor.visit(*this); }
};

// 具体访问者实现了同一算法的多个版本，而且该算法能与所有具体类进行交互。
//
// 访问者模式在复杂对象结构（例如组合树）上使用时能发挥最大作用。在这种情
// 况下，它可以存储算法的一些中间状态，并同时在结构中的不同对象上执行访问
// 者方法。这可能会非常有帮助。
class XMLExportVisitor : public Visitor {
 public:
  void visit(Dot&) override { cout << "Export Dot\n"; }
  void visit(Circle&) override { cout << "Export Circle\n"; }
  void visit(Rectangle&) override { cout << "Export Rectangle\n"; }
  void visit(CompoundShape&) override { cout << "Export CompoundShape\n"; }
};

struct App {
  App() {
    unique_ptr<Visitor> visitor = make_unique<XMLExportVisitor>();
    vector<unique_ptr<Shape>> all_shapes;
    {
      all_shapes.emplace_back(make_unique<Circle>());
      all_shapes.emplace_back(make_unique<Rectangle>());
      all_shapes.emplace_back(make_unique<Dot>());
      all_shapes.emplace_back(make_unique<CompoundShape>());
    }
    for (auto&& shape : all_shapes)
      shape->accept(*visitor);
  }
};

int main() {
  App();
  return 0;
}
