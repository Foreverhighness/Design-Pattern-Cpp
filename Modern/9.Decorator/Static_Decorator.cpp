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

template <typename T>
class ColoredShape : public T {
  static_assert(is_base_of_v<Shape, T>, "Template argument must be a Shape");

 protected:
  string color_;

 public:
  ColoredShape() = default;
  template <typename... Args>
  ColoredShape(string_view color, Args&&... args) : T(std::forward<Args>(args)...), color_(color) {}
  string str() const override { return T::str() + " has the color " + color_; }
};
template <typename T>
class TransparentShape : public T {
  static_assert(is_base_of_v<Shape, T>, "Template argument must be a Shape");

 protected:
  uint8_t transparency_;

 public:
  TransparentShape() = default;
  template <typename... Args>
  TransparentShape(const uint8_t transparency, Args&&... args)
      : T(std::forward<Args>(args)...), transparency_(transparency) {}
  string str() const override {
    return T::str() + " has " + to_string(static_cast<double>(transparency_) / 255. * 100.) + "% transparency";
  }
};

void test() {
  ColoredShape<TransparentShape<Circle>> circle{"blue", 23, 2};
  cout << circle.str() << ".\n";
  // can call circle's own members
  circle.resize(3);
  cout << circle.str() << ".\n";

  ColoredShape<TransparentShape<Square>> square = {"red", 51, 5};
  cout << square.str() << ".\n";
  ColoredShape<ColoredShape<Square>> ccs = {"red", "blue", 3};
  cout << ccs.str() << ".\n";
  // A square with side 5 has 20% transparency has the color red.
}
int main() {
  test();
  return 0;
}
