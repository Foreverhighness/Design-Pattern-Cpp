#include <iostream>
#include <string>
#include <memory>
using namespace std;
namespace generic {
namespace aaa {
template <typename T> struct FooBind;
template <typename T>
struct FooBase {
    int mark;
    virtual void bar(T val) = 0;
    virtual ~FooBase() { cout << "跳走了" << endl; }
    // FooBind<T> bind(FooBase&  other) { return FooBind<T>(*this, other); }
    // FooBind<T> bind(FooBase&& other) { return FooBind<T>(std::move(*this), other); }
    FooBind<T> bind(FooBase*  other) { return FooBind<T>(this, other); }
};
struct FooPlus1 : FooBase<int> { void bar(int val) override { cout << val + 1 << endl; } };
struct FooPlus2 : FooBase<int> { void bar(int val) override { cout << val + 2 << endl; } };
struct FooPlus3 : FooBase<int> { void bar(int val) override { cout << val + 3 << endl; } };

template <typename T>
struct FooBind : FooBase<T> {
    shared_ptr<FooBase<T>> a, b;
    // explicit FooBind(FooBase<T>&  a, FooBase<T>&  b) : a(&a), b(&b) {}//a(make_shared<FooBase<T>>(a)), b(make_shared<FooBase<T>>(b)) {}
    // explicit FooBind(FooBase<T>&& a, FooBase<T>&  b) : a(make_shared<FooBase>(a)), b(make_shared<FooBase>(b)) {}
    // explicit FooBind(FooBase<T>&  a, FooBase<T>&& b) : a(make_shared<FooBase>(a)), b(make_shared<FooBase>(b)) {}
    // explicit FooBind(FooBase<T>&& a, FooBase<T>&& b) : a(make_shared<FooBase>(a)), b(make_shared<FooBase>(b)) {}
    explicit FooBind(FooBase<T>* a, FooBase<T>* b) : a(a), b(b) {}
    void bar(T val) override {
        a->bar(val);
        b->bar(val);
    }
};
} // namespace aaa
namespace bbb {
template <typename T> struct Binded;
template <typename T>
struct Base {
    int mark;
    Base() : mark(0) {}
    virtual void bar(T val) = 0;
    virtual ~Base() { cout << "mark: " << mark << " 溜了溜了" << endl; }
    Binded<T> bind(Base<T>&  other) { return Binded<T>(*this, other); }
    Binded<T> bind(Base<T>&& other) { return Binded<T>(*this, other); }
};
struct Plus1 : Base<int> { Plus1() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base<int> { Plus2() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base<int> { Plus3() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
template <typename T>
struct Binded : Base<T> {
    Base<T>& a;
    Base<T>& b;
    explicit Binded(Base<T>& a, Base<T>&  b) : Base<T>(), a(a), b(b) {this->mark = 4;}
    // explicit Binded(Base<T>& a, Base<T>&& b) : Base<T>(), a(a), b(b) {this->mark = 6;}
    void bar(T val) override {
        a.bar(val); b.bar(val);
    }
};
} // namespace bbb
namespace ccc {
template <typename T> struct Binded;
template <typename T>
struct Base {
    int mark;
    Base() : mark(1) {}
    virtual void bar(T val) = 0;
    virtual ~Base() { cout << "mark: " << mark << " 溜了溜了" << endl; }
    // Binded<T> bind(Base<T>&  other) { return this->bind(std::move(other)); }
    Binded<T> bind(unique_ptr<Base<T>>&& other) { return Binded<T>(forward<unique_ptr<Base<T>>>(unique_ptr<Base<T>>(this)), forward<unique_ptr<Base<T>>>(other)); }
    // Binded<T> bind(Base<T>&& other) { return Binded<T>(std::move(*this), std::forward<Base<T>>(other)); }
};
struct Plus1 : Base<int> { Plus1() : Base<int>() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base<int> { Plus2() : Base<int>() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base<int> { Plus3() : Base<int>() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
template <typename T>
struct Binded : Base<T> {
    unique_ptr<Base<T>> a;
    unique_ptr<Base<T>> b;
    explicit Binded(unique_ptr<Base<T>>&& a, unique_ptr<Base<T>>&& b) : a(forward<unique_ptr<Base<T>>>(a)), b(forward<unique_ptr<Base<T>>>(b)) {this->mark = 4;}
    // explicit Binded(Base<T>&  a, Base<T>&   b) : Binded(std::move(a), std::move(b)) {this->mark = 5;}
    // explicit Binded(Base<T>&& a, Base<T>&&  b) : Base<T>(), a(make_unique<Base<T>>(a)), b(make_unique<Base<T>>(b)) {this->mark = 6;}
    // explicit Binded(Base<T>& a, Base<T>&& b) : Base<T>(), a(a), b(b) {this->mark = 6;}
    void bar(T val) override {
        a->bar(val); b->bar(val);
    }
};
} // namespace ccc
} // namespace generic

namespace specific {
namespace worked_using_raw_pointer {
struct Binded;
struct Base {
    int mark = 0;
    virtual void bar(int val) = 0;
    virtual ~Base() { cout << "mark: " << mark << " 溜了溜了" << endl; }
    // Binded<T> bind(Base<T>&  other) { return this->bind(std::move(other)); }
    // Binded bind(Base&& other); // { return Binded(*this, other); }
    // Binded<T> bind(Base<T>&& other) { return Binded<T>(std::move(*this), std::forward<Base<T>>(other)); }
    Binded* bind(Base* other); // { return make_unique<Binded>(*this, other); }
};
struct Binded : Base {
    Base* a;
    Base* b;
    // Base &a;
    // Base &b;
    ~Binded() {
        // cout << "mark: " << a->mark << "跳走了" << endl;
        delete a;
        // cout << "mark: " << b->mark << "跳走了" << endl;
        delete b;
    }
    explicit Binded(Base* a, Base* b) : a(a), b(b) { this->mark = 4; }
    // explicit Binded(Base&  a, Base&  b) : a(a), b(b) { this->mark = 5; }
    // explicit Binded(Base&  a, Base&  b) : Binded(std::move(a), std::move(b)) { this->mark = 5; }
    // explicit Binded(Base&& a, Base&& b) : a(a), b(b) { this->mark = 6; }
    // explicit Binded(Base<T>& a, Base<T>&& b) : Base<T>(), a(a), b(b) { this->mark = 6; }
    void bar(int val) override {
        // a.bar(val); b.bar(val);
        a->bar(val); b->bar(val);
    }
};
Binded* Base::bind(Base* other) {
    // other = *this;
    return new Binded(std::move(this), std::move(other));
    // return Binded(*this, other);
}
struct Plus1 : Base { Plus1() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base { Plus2() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base { Plus3() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
} // worked_using_raw_pointer
namespace bbb {
struct Binded;
struct Base {
    int mark = 0;
    virtual void bar(int val) = 0;
    virtual ~Base() { cout << "mark: " << mark << " 溜了溜了" << endl; }
    // Binded bind(unique_ptr<Base> other);
    Binded* bind(Base* other);
};
struct Binded : Base {
    unique_ptr<Base> a;
    unique_ptr<Base> b;
    explicit Binded(unique_ptr<Base> a, unique_ptr<Base> b) : a(std::move(a)), b(std::move(b)) { this->mark = 4; }
    void bar(int val) override {
        a->bar(val); b->bar(val);
    }
};
// Binded Base::bind(unique_ptr<Base> other) {
//     return Binded(std::move(unique_ptr<Base>(this)), std::move(other));
// }
Binded* Base::bind(Base* other) {
    return new Binded(std::move(unique_ptr<Base>(this)), std::move(unique_ptr<Base>(other)));
}
struct Plus1 : Base { Plus1() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base { Plus2() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base { Plus3() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
} // namespace bbb
namespace ccc {
struct Binded;
struct Base {
    int mark = 0;
    virtual void bar(int val) = 0;
    virtual ~Base() { cout << "mark: " << mark << " 溜了溜了" << endl; }
    // Binded bind(unique_ptr<Base> other);
    unique_ptr<Binded> bind(Base* other);
};
struct Binded : Base {
    unique_ptr<Base> a;
    unique_ptr<Base> b;
    explicit Binded(unique_ptr<Base> a, unique_ptr<Base> b) : a(std::move(a)), b(std::move(b)) { this->mark = 4; }
    void bar(int val) override {
        a->bar(val); b->bar(val);
    }
};
// Binded Base::bind(unique_ptr<Base> other) {
//     return Binded(std::move(unique_ptr<Base>(this)), std::move(other));
// }
unique_ptr<Binded> Base::bind(Base* other) {
    return make_unique<Binded>(std::move(unique_ptr<Base>(this)), std::move(unique_ptr<Base>(other)));
}
struct Plus1 : Base { Plus1() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base { Plus2() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base { Plus3() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
} // namespace ccc
namespace worked_using_shared_ptr_pointer {
int time = 0;
struct Binded;
struct Base : enable_shared_from_this<Base>{
    int mark = 0, t;
    Base() : mark(0), t(++time) {}
    string info() { return "t: " + to_string(t) + " mark: " + to_string(mark); }
    virtual ~Base() { cout << info() << " 溜了溜了" << endl; }
    virtual void bar(int val) = 0;
    shared_ptr<Binded> bind(shared_ptr<Base> other);
};
shared_ptr<Binded> Base::bind(shared_ptr<Base> other) { return make_shared<Binded>(shared_from_this(), other); }
struct Binded : Base {
    shared_ptr<Base> a;
    shared_ptr<Base> b;
    explicit Binded(shared_ptr<Base> a, shared_ptr<Base> b) : Base(), a(a), b(b) { this->mark = 4; }
    void bar(int val) override { a->bar(val); b->bar(val); }
};
struct Plus1 : Base { Plus1() : Base() { mark = 1;} void bar(int val) override { cout << val + 1 << endl; } };
struct Plus2 : Base { Plus2() : Base() { mark = 2;} void bar(int val) override { cout << val + 2 << endl; } };
struct Plus3 : Base { Plus3() : Base() { mark = 3;} void bar(int val) override { cout << val + 3 << endl; } };
void dfs(Base* p, int val) {
    cout << p->info() << " ---> " << val << endl;
    if (typeid(*p) == typeid(Binded)) {
        auto b = static_cast<Binded*>(p);
        dfs(b->a.get(), b->a.use_count());
        dfs(b->b.get(), b->b.use_count());
    }
}
void test() {
    auto b = (make_shared<Plus1>())->bind(make_shared<Plus2>())->bind(make_shared<Plus3>());
    cout << b->mark << endl;
    cout << b->a->mark << endl;
    cout << b->b->mark << endl << endl;
    b->bar(1);
    cout << endl;
    dfs(b.get(), b.use_count());
}
} // namespace worked_using_shared_ptr_pointer
namespace untest_ref {
int time = 0;
struct Binded;
struct Base {
    int mark = 0, t;
    Base() : mark(0), t(++time) {}
    string info() { return "t: " + to_string(t) + " mark: " + to_string(mark); }
    virtual ~Base() { cout << info() << " 溜了溜了" << endl; }
    virtual void bar(int val) const  = 0;
    friend Binded operator&&(Base& lhs, Base& rhs);
    // friend Binded operator&&(const Base&& lhs, const Base&& rhs);
};
struct Plus1 : Base { Plus1() : Base() { mark = 1;} void bar(int val) const override { cout << val + 1 << endl; } };
struct Plus2 : Base { Plus2() : Base() { mark = 2;} void bar(int val) const override { cout << val + 2 << endl; } };
struct Plus3 : Base { Plus3() : Base() { mark = 3;} void bar(int val) const override { cout << val + 3 << endl; } };

struct Binded : Base {
    Base& a;
    Base& b;
    Binded(Base& a, Base& b) : Base(), a(a), b(b) { this->mark = 4; }
    // Binded(Base&& a, Base&& b) : Base(), a(a), b(b) { this->mark = 4; }
    void bar(int val) const override { a.bar(val); b.bar(val); }
};
Binded operator&&(Base& lhs, Base& rhs) {
    return Binded(lhs, rhs);
}
// Binded operator&&(Base&& lhs, Base&& rhs) {
//     return Binded(std::move(lhs), std::move(rhs));
// }
void test() {
    auto p1 = Plus1(); 
    auto p2 = Plus2();
    auto p3 = Plus3();
    auto b = p1 && p2;
    // auto b = Plus1() && Plus2() && Plus3();
    b.bar(1);
}
} // namespace untest_ref
} // namespace specific

int main() {
    using namespace specific::untest_ref;
    test();
    return 0;
}

// FooBind<int> fb = FooPlus1().bind(FooPlus2()).bind(FooPlus3());
// FooBind<int> fb = (new FooPlus1())->bind(new FooPlus2()).bind(new FooPlus3());
// fb.bar(1);
// fb.bar(1);
// Plus1 p1;
// Plus2 p2;
// Plus3 p3;
// Binded<int> tmp = p1.bind(p2);
// Binded<int> b2 = tmp.bind(p3);
// b2.bar(1);
// auto b1 = Plus1().bind(Plus2()).bind(Plus3());
// auto b1 = ((new Plus1()))->bind(std::move(unique_ptr<Base>(new Plus2())))->bind(unique_ptr<Base>(new Plus3()));
// auto b1 = unique_ptr<Binded>((new Plus1)->bind(new Plus2)->bind(new Plus3));
// cout << b2.mark << endl;
// cout << b2.a.mark << endl;
// cout << b2.b.mark << endl;
// auto b1 = (make_shared<Plus1>())->bind(make_shared<Plus2>())->bind(make_shared<Plus3>());
// cout << b1->mark << endl;
// cout << b1->a->mark << endl;
// cout << b1->b->mark << endl << endl;
// b1->bar(1);