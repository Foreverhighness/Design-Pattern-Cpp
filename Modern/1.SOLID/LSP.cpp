#include <iostream>
using namespace std;
class Rectangle {
   protected:
    int _width, _height;

   public:
    Rectangle(int width, int height) : _width(width), _height(height) {}
    int width() const { return _width; }
    int height() const { return _height; }
    virtual void setWidth(const int width) { this->_width = width; }
    virtual void setHeight(const int height) { this->_height = height; }
    int area() const { return _height * _width; }
};

class Square : public Rectangle {
    public:
    Square(int width) : Rectangle(width, width) {}
    void setWidth(const int width) override { this->_width = this->_height = width; }
    void setHeight(const int height) override { this->_height = this->_width = height; }
};

void process(Rectangle& r) {
    int w = r.width();
    r.setHeight(10);
    cout << "expected area = " << w * 10 << endl;
    cout << "got " << r.area() << endl;
}

int main() {
    Square s(5);
    process(s);
    return 0;
}
