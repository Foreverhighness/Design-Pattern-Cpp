#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using std::string;
using std::unique_ptr;
using std::vector;
// 基础原型。
struct Shape {
    int x, y;
    string color;
    Shape(const int x, const int y, const string& color) : x{x}, y{y}, color{color} {}
    Shape(const Shape& other) : x{other.x}, y{other.y}, color{other.color} {};
    virtual ~Shape(){};
    virtual unique_ptr<Shape> clone() const = 0;
    virtual double area() const = 0;
};

class Rectangle : public Shape {
    int _width, _height;

   public:
    Rectangle(const int x, const int y, const string& color, const int width, const int height)
        : Shape{x, y, color}, _width{width}, _height{height} {}
    Rectangle(const Rectangle& other) : Shape{other}, _width{other._width}, _height{other._height} {}
    unique_ptr<Shape> clone() const override { return unique_ptr<Shape>(new Rectangle(*this)); }
    double area() const override { return 1.0 * _width * _height; }
};

class Circle : public Shape {
    int radius;

   public:
    Circle(const int x, const int y, const string& color, const int radius) : Shape{x, y, color}, radius{radius} {}
    Circle(const Circle& other) : Shape{other}, radius{other.radius} {}
    unique_ptr<Shape> clone() const override { return unique_ptr<Shape>(new Circle(*this)); }
    double area() const override { return M_PI * radius * radius; }
};

class ShapeCache {
    inline static vector<unique_ptr<Shape>> shapes;

   public:
    static unique_ptr<Shape> get(const int id) { return ShapeCache::shapes.at(id)->clone(); }
    static void setDefault() {
        shapes.clear();
        ShapeCache::shapes.emplace_back(new Circle{0, 0, "Circle", 1});
        ShapeCache::shapes.emplace_back(new Rectangle{0, 0, "Rectangle", 1, 2});
    }
};

class App {
   public:
    App() {
        ShapeCache::setDefault();
        vector<unique_ptr<Shape>> shapes;
        for (int i = 0; i < 2; ++i) {
            shapes.push_back(ShapeCache::get(i));
        }
        for (auto&& shape : shapes) {
            std::cout << shape->color << "\t" << shape->area() << std::endl;
        }
    }
};

int main() {
    App();
    return 0;
}