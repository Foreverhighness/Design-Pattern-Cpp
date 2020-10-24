#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;
// There is the external libary which you can't modify.
// ----------------------------------------------------------------
struct Point {
    int x, y;
    bool operator<(const Point& other) const { return x == other.x ? y < other.y : x < other.x; }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};
void DrawPoint(vector<Point>::iterator begin, vector<Point>::iterator end) {
    for (auto it = begin; it != end; ++it)
        cout << "Drawing (" << it->x << ", " << it->y << ")\n";
}
// ----------------------------------------------------------------

struct Line {
    Point start, end;
    Line(const Point& s, const Point& e) {
        if (e < s)
            start = s, end = e;
        else
            start = e, end = s;
    }
};

struct VecObject {
    virtual ~VecObject() = default;
    virtual vector<Line>::iterator begin() = 0;
    virtual vector<Line>::iterator end() = 0;
};

class VecRectangle : public VecObject {
    vector<Line> lines_;

   public:
    VecRectangle(int x, int y, int width, int height) {
        lines_.emplace_back(Point{x, y}, Point{x + width, y});
        lines_.emplace_back(Point{x, y}, Point{x, y + height});
        lines_.emplace_back(Point{x + width, y}, Point{x + width, y + height});
        lines_.emplace_back(Point{x, y + height}, Point{x + width, y + height});
    }
    vector<Line>::iterator begin() override { return lines_.begin(); }
    vector<Line>::iterator end() override { return lines_.end(); }
};

struct Line2PointAdapter {
    using Points = std::vector<Point>;
    Line2PointAdapter(Line& line) {
        int left = min(line.start.x, line.end.x);
        int right = max(line.start.x, line.end.x);
        int top = min(line.start.y, line.end.y);
        int bottom = max(line.start.y, line.end.y);
        int dx = right - left;
        int dy = bottom - top;

        // only vertical or horizontal lines
        if (dx == 0) {
            // vertical
            for (int y = top; y <= bottom; ++y)
                points_.emplace_back(Point{left, y});
        } else if (dy == 0) {
            for (int x = left; x <= right; ++x)
                points_.emplace_back(Point{x, top});
        }
    }
    virtual ~Line2PointAdapter() = default;
    virtual vector<Point>::iterator begin() { return points_.begin(); }
    virtual vector<Point>::iterator end() { return points_.end(); }

   private:
    Points points_;
};

void test() {
    // vector<Point> ps{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}};
    // DrawPoint(ps.begin(), ps.end());
    vector<shared_ptr<VecObject>> vectorObjects{make_shared<VecRectangle>(0, 0, 3, 3),
                                                make_shared<VecRectangle>(1, 1, 1, 1)};
    for (auto&& obj : vectorObjects) {
        for (auto&& line : *obj) {
            Line2PointAdapter l2p{line};
            DrawPoint(l2p.begin(), l2p.end());
        }
    }
}
int main() {
    test();
    return 0;
}