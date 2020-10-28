#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
enum struct Color { Red, Green, Blue };
enum struct Size { Small, Medium, Large };

struct Product {
    string name;
    Color color;
    Size size;
};

struct ProductFilter {
    using Items = vector<Product*>;
    Items by_color(Items&, Color);
};
// 原函数
ProductFilter::Items ProductFilter::by_color(Items& items, Color color) {
    Items ret;
    for (auto&& item : items) {
        if (item->color == color)
            ret.push_back(item);
    }
    return ret;
}
template <typename T>
struct AndSpecification;
// 使用辅助类来促成开闭原则
template <typename T>
struct Specification {
    virtual ~Specification() {}
    virtual bool is_satisfied(T* item) const = 0;
    template <typename U>
    friend AndSpecification<U> operator&&(const Specification<U>& lhs, const Specification<U>& rhs);
    // template <typename U>
    // friend AndSpecification<U> operator&&(Specification<U>&& lhs, Specification<U>&& rhs);
};

template <typename T>
struct Filter {
    using Items = vector<T*>;
    using Spec = Specification<T>;
    virtual Items filter(Items& items, Spec& spec) const = 0;
};

struct BetterFilter : Filter<Product> {
    Items filter(Items& items, Spec& spec) const override;
};

struct ColorSpecification : Specification<Product> {
    Color color;

    explicit ColorSpecification(const Color color) : color{color} {}
    bool is_satisfied(Product* item) const override;
};
struct SizeSpecification : Specification<Product> {
    Size size;

    explicit SizeSpecification(const Size size) : size{size} {}
    bool is_satisfied(Product* item) const override;
};

template <typename T>
struct AndSpecification : Specification<T> {
    using Spec = Specification<T>;
    const Spec& first;
    const Spec& second;

    AndSpecification(const Spec& first, const Spec& second) : first(first), second(second) {}
    // AndSpecification(Spec&& first, Spec&& second) : first(first), second(second) {}
    // AndSpecification(Spec& first, Spec& second) : first(first), second(second) {}
    bool is_satisfied(T* item) const override;
};

int main() {
    Product apple{"Apple", Color::Red, Size::Small};
    Product tree{"Tree", Color::Green, Size::Large};
    Product house{"House", Color::Blue, Size::Large};
    Product leaf{"Leaf", Color::Green, Size::Small};

    vector<Product*> all{&apple, &tree, &house, &leaf};

    BetterFilter bf;

    ColorSpecification green(Color::Green);
    SizeSpecification large(Size::Large);
    // auto green_things = bf.filter(all, green);
    // for (auto&& item : green_things)
    //     std::cout << item->name << " is green" << std::endl;
    // auto green_and_big = green && large;
    // AndSpecification<Product> green_and_big(green, large);
    Specification<Product>&& rval_green = ColorSpecification(Color::Green);
    Specification<Product>&& rval_large = SizeSpecification(Size::Large);
    AndSpecification<Product> green_and_big{rval_green && rval_large};
    // The next line will generate a runtime error because of a lifetime error
    // AndSpecification<Product> green_and_big{ColorSpecification(Color::Green) && SizeSpecification(Size::Large)};
    auto things = bf.filter(all, green_and_big);
    for (auto&& item : things)
        std::cout << item->name << " is green and big" << std::endl;
}

BetterFilter::Items BetterFilter::filter(Items& items, Spec& spec) const {
    Items ret;
    for (auto&& item : items) {
        if (spec.is_satisfied(item))
            ret.push_back(item);
    }
    return ret;
}
template <typename T>
AndSpecification<T> operator&&(const Specification<T>& lhs, const Specification<T>& rhs) {
    return {lhs, rhs};
}
// template <typename T>
// AndSpecification<T> operator&&(Specification<T>&& lhs, Specification<T>&& rhs) {
//     return {lhs, rhs};
// }
bool ColorSpecification::is_satisfied(Product* item) const {
    return item->color == color;
}
bool SizeSpecification::is_satisfied(Product* item) const {
    return item->size == size;
}
template <typename T>
bool AndSpecification<T>::is_satisfied(T* item) const {
    return first.is_satisfied(item) && second.is_satisfied(item);
}