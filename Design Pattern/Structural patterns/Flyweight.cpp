#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <memory>
using namespace std;
using canvas = ostream;
enum class Color { red, green, blue, sz };
const string_view color_map[static_cast<size_t>(Color::sz)] = {"red", "green", "blue"};

// 享元类包含一个树的部分状态。这些成员变量保存的数值对于特定树而言是唯一
// 的。例如，你在这里找不到树的坐标。但这里有很多树木之间所共有的纹理和颜
// 色。由于这些数据的体积通常非常大，所以如果让每棵树都其进行保存的话将耗
// 费大量内存。因此，我们可将纹理、颜色和其他重复数据导出到一个单独的对象
// 中，然后让众多的单个树对象去引用它。
class TreeType {
    string type_;
    Color color_;
    int texture_;

   public:
    TreeType(string_view type, Color color, int texture) : type_(type), color_(color), texture_(texture) {}
    void draw(canvas& cvs, int x, int y) {
        cvs << "Draw a " << type_ << "tree with " << color_map[static_cast<size_t>(color_)] << " color and " << texture_
            << "texture at"
            << "(" << x << ", " << y << ")\n";
    }
};

// 享元工厂决定是否复用已有享元或者创建一个新的对象。
class TreeFactory {
   public:
    using Key = tuple<string, Color, int>;
    static TreeType* getTreeType(string_view type, Color color, int texture) {
        auto key = make_tuple(static_cast<string>(type), color, texture);
        if (auto it = flyweight.find(key); it != flyweight.end())
            return it->second.get();
        auto ret = new TreeType(type, color, texture);
        flyweight[key] = unique_ptr<TreeType>(ret);
        return ret;
    }

    inline static map<Key, unique_ptr<TreeType>> flyweight;
};

// 情景对象包含树状态的外在部分。程序中可以创建数十亿个此类对象，因为它们
// 体积很小：仅有两个整型坐标和一个引用成员变量。
class Tree {
    int x_, y_;
    TreeType* treetype_;

   public:
    Tree(int x, int y, TreeType* type) : x_(x), y_(y), treetype_(type) {}
    void draw(canvas& cvs) { treetype_->draw(cvs, x_, y_); }
};

// 树（Tree）和森林（Forest）类是享元的客户端。如果不打算继续对树类进行开
// 发，你可以将它们合并。
class Forest {
    vector<unique_ptr<Tree>> trees_;

   public:
    void plantTree(int x, int y, string_view type, Color color, int texture) {
        trees_.emplace_back(new Tree(x, y, TreeFactory::getTreeType(type, color, texture)));
    }
    void draw(canvas& cvs) {
        for (auto&& tree : trees_)
            tree->draw(cvs);
    }
};

struct App {
    App() {
        Forest f;
        f.plantTree(0 ,0, "t1", Color::red, 1);
        f.plantTree(0 ,0, "t2", Color::green, 2);
        f.plantTree(0 ,0, "t3", Color::blue, 3);
        f.plantTree(0 ,0, "t1", Color::red, 1);
        cout << TreeFactory::flyweight.size() << endl;
    }
};

int main() {
    App();
    return 0;
}
