#include <iostream>
#include <memory>
using namespace std;

struct Render {
    virtual ~Render() = default;
    virtual void render_circle(double x, double y, double radius) = 0;
};

struct VectorRenderer : public Render {
    void render_circle(double x, double y, double radius) override {
        cout << "Drawing a vector circle of radius " << radius << endl;
    }
};
struct RasterRenderer : public Render {
    void render_circle(double x, double y, double radius) override {
        cout << "Rasterizing circle of radius " << radius << endl;
    }
};

struct Shape {
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;

   protected:
    unique_ptr<Render> renderer_;
    Shape(unique_ptr<Render> renderer) : renderer_(std::move(renderer)) {}
};

struct Circle : public Shape {
    double x, y, radius;

    Circle(unique_ptr<Render> renderer, double x, double y, double radius)
        : Shape(std::move(renderer)), x(x), y(y), radius(radius) {}
    void draw() const override { renderer_->render_circle(x, y, radius); }
    void resize(double factor) override { radius *= factor; }
};

void test() {
    Shape&& rv_Shape = Circle(make_unique<VectorRenderer>(), 5, 5, 5);
    rv_Shape.draw();
    rv_Shape.resize(2);
    rv_Shape.draw();
    unique_ptr<Shape> raster_circle = make_unique<Circle>(make_unique<RasterRenderer>(), 5, 5, 5);
    raster_circle->draw();
    raster_circle->resize(2);
    raster_circle->draw();
}
int main() {
    test();
    return 0;
}