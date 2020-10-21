#include <cmath>
#include <iostream>
using namespace std;
class Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
    struct PointFactory {
        static Point NewCartesian(double x, double y) { return {x, y}; }
        static Point NewPolar(double r, double theta) { return {r * cos(theta), r * sin(theta)}; }
    };

   public:
    using Factory = PointFactory;
    friend ostream& operator<<(ostream& os, const Point& p);
};
ostream& operator<<(ostream& os, const Point& p) {
    os << "{" << p.x << ", " << p.y << "}";
    return os;
}
void test() {
    auto p0 = Point::Factory::NewCartesian(1, 2);
    cout << p0 << endl;
    auto p1 = Point::Factory::NewPolar(1, M_PI / 4);
    cout << p1 << endl;
}
int main() {
    test();
    return 0;
}