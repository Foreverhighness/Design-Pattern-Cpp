#include <iostream>
#include <memory>
using namespace std;

#include <boost/lexical_cast.hpp>  // comment this header to use std::to_string;
#ifdef BOOST_LEXICAL_CAST_INCLUDED
#define to_string boost::lexical_cast<std::string>
#endif

struct Shape {
  virtual ~Shape() = default;
  virtual string str() const = 0;
};
class Circle : public Shape {
  double radius_;

 public:
  Circle() = default;
  explicit Circle(const double radius) : radius_(radius) {}
  string str() const override { return "A circle of radius "s + to_string(radius_); }

  void resize(const double factor) { radius_ *= factor; }
};
class Square : public Shape {
  double side_;

 public:
  Square() = default;
  explicit Square(const double side) : side_{side} {}
  string str() const override { return "A square with side "s + to_string(side_); }
};

class ColoredShape : public Shape {
  unique_ptr<Shape> shape_;
  string color_;

 public:
  ColoredShape(unique_ptr<Shape> shape, string_view color) : shape_(std::move(shape)), color_(color) {}
  string str() const override { return shape_->str() + " has the color " + color_; }
};
class TransparentShape : public Shape {
  unique_ptr<Shape> shape_;
  uint8_t transparency_;

 public:
  TransparentShape(unique_ptr<Shape> shape, uint8_t transparency)
      : shape_(std::move(shape)), transparency_(transparency) {}
  string str() const override {
    return shape_->str() + " has " + to_string(static_cast<double>(transparency_) / 255. * 100.) + "% transparency";
  }
};

void test() {
  unique_ptr<Shape> circle{make_unique<Circle>(0.5)};
  unique_ptr<Shape> redCircle{make_unique<ColoredShape>(std::move(circle), "red")};
  cout << redCircle->str() << ".\n";
  // prints "A circle of radius 0.5 has the color red"

  unique_ptr<Shape> square{make_unique<Square>(3)};
  unique_ptr<Shape> demiSquare{make_unique<TransparentShape>(std::move(square), 85)};
  cout << demiSquare->str() << ".\n";
  // A square with side 3 has 33.333% transparency
  unique_ptr<Shape> myCircle{
      make_unique<TransparentShape>(make_unique<ColoredShape>(make_unique<Circle>(23), "green"), 64)};
  cout << myCircle->str() << ".\n";
  // A circle of radius 23 has the color green has 25.098% transparency
}
int main() {
  test();
  return 0;
}
