#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;

#include <boost/functional/hash.hpp>

// There is the external libary which you can't modify.
// ----------------------------------------------------------------
struct Point {
    int x, y;
    bool operator<(const Point& other) const { return x == other.x ? y < other.y : x < other.x; }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }

    friend std::size_t hash_value(const Point& obj) {
        std::size_t seed = 0x1E44943F;
        boost::hash_combine(seed, obj.x);
        boost::hash_combine(seed, obj.y);
        return seed;
    }
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

    friend std::size_t hash_value(const Line& obj) {
        std::size_t seed = 0x4D274623;
        boost::hash_combine(seed, obj.start);
        boost::hash_combine(seed, obj.end);
        return seed;
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

struct Line2PointCacheAdapter {
    using Points = std::vector<Point>;
    Line2PointCacheAdapter(Line& line) {
        boost::hash<Line> hashFunc;
        hash_code_ = hashFunc(line);
        if (cache.find(hash_code_) != cache.end())
            return;
        int left = min(line.start.x, line.end.x);
        int right = max(line.start.x, line.end.x);
        int top = min(line.start.y, line.end.y);
        int bottom = max(line.start.y, line.end.y);
        int dx = right - left;
        int dy = bottom - top;

        Points points;
        // only vertical or horizontal lines
        if (dx == 0) {
            // vertical
            for (int y = top; y <= bottom; ++y)
                points.emplace_back(Point{left, y});
        } else if (dy == 0) {
            for (int x = left; x <= right; ++x)
                points.emplace_back(Point{x, top});
        }
        cout << "Initialized" << endl;
        cache[hash_code_] = points;
    }
    virtual ~Line2PointCacheAdapter() = default;
    virtual vector<Point>::iterator begin() { return cache[hash_code_].begin(); }
    virtual vector<Point>::iterator end() { return cache[hash_code_].end(); }

   private:
    std::size_t hash_code_;
    inline static unordered_map<std::size_t, Points> cache;
};

void test() {
    vector<shared_ptr<VecObject>> vectorObjects{make_shared<VecRectangle>(0, 0, 3, 3),
                                                make_shared<VecRectangle>(1, 1, 1, 1)};
    for (auto&& obj : vectorObjects) {
        for (auto&& line : *obj) {
            Line2PointCacheAdapter l2p{line};
            DrawPoint(l2p.begin(), l2p.end());
        }
    }
    for (auto&& obj : vectorObjects) {
        for (auto&& line : *obj) {
            Line2PointCacheAdapter l2p{line};
            DrawPoint(l2p.begin(), l2p.end());
        }
    }
}
int main() {
    test();
    return 0;
}