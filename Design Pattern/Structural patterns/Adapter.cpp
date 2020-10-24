#include <cmath>
#include <iostream>
#include <memory>
using namespace std;
// 假设你有两个接口相互兼容的类：圆孔（Round­Hole）和圆钉（Round­Peg）。
class RoundPeg {
   protected:
    double radius_;

   public:
    RoundPeg() = default;
    RoundPeg(double val) : radius_(val) {}
    double get_radius() const { return radius_; }
};
class RoundHole {
   protected:
    double radius_;

   public:
    RoundHole(double val) : radius_(val) {}
    bool is_satisfied(const RoundPeg& peg) const { return radius_ >= peg.get_radius(); }
    int get_radius() const { return radius_; }
};
// 但还有一个不兼容的类：方钉（Square­Peg）。
class SquarePeg {
   protected:
    double size_;

   public:
    SquarePeg() = default;
    SquarePeg(double val) : size_(val) {}
    double get_size() const { return size_; }
};
// 适配器类让你能够将方钉放入圆孔中。它会对 RoundPeg 类进行扩展，以接收适
// 配器对象作为圆钉。
class SquarePeg2RoundHoleAdapter : public RoundPeg {
    shared_ptr<SquarePeg> p_rpeg_;

   public:
    SquarePeg2RoundHoleAdapter(shared_ptr<SquarePeg>& p) : p_rpeg_(p) { radius_ = p_rpeg_->get_size() / sqrt(2); }
};

struct App {
    App() {
        shared_ptr<RoundHole> hole = make_shared<RoundHole>(5);
        shared_ptr<RoundPeg> rpeg = make_shared<RoundPeg>(5);
        cout << boolalpha << hole->is_satisfied(*rpeg) << endl;
        shared_ptr<SquarePeg> small_sqpeg = make_shared<SquarePeg>(5), large_sqpeg = make_shared<SquarePeg>(10);

        shared_ptr<RoundPeg> small_sqpeg_adapter = make_shared<SquarePeg2RoundHoleAdapter>(small_sqpeg),
                             large_sqpeg_adapter = make_shared<SquarePeg2RoundHoleAdapter>(large_sqpeg);

        cout << boolalpha << hole->is_satisfied(*small_sqpeg_adapter) << endl;
        cout << boolalpha << hole->is_satisfied(*large_sqpeg_adapter) << endl;
    }
};

int main() {
    App();
    return 0;
}