#include <iostream>
#include <memory>
using namespace std;
namespace pimpllib {
template <typename PImpl>
class pimpl {
   private:
    unique_ptr<PImpl> impl_;

   public:
    ~pimpl();
    pimpl();

    template <typename... Args>
    pimpl(Args&&... args);

    PImpl* operator->() const;
    PImpl& operator*() const;
};

template <typename PImpl>
pimpl<PImpl>::pimpl() : impl_(make_unique<PImpl>()) {}

template <typename PImpl>
pimpl<PImpl>::~pimpl() = default;

template <typename PImpl>
template <typename... Args>
pimpl<PImpl>::pimpl(Args&&... args) : impl_(make_unique<PImpl>(std::forward<Args>(args)...)) {}

template <typename PImpl>
PImpl* pimpl<PImpl>::operator->() const {
    return &this->operator*();
}
template <typename PImpl>
PImpl& pimpl<PImpl>::operator*() const {
    return *impl_;
}
}  // namespace pimpllib

struct Person {
    Person(string_view name);
    ~Person() noexcept;
    string get_name() const;
    void greet();

   protected:
    struct impl;
    pimpllib::pimpl<impl> impl_;
};

struct Person::impl {
    impl(string_view name) : name_(name) {}
    string getName(const Person&) { return name_; }

    string name_;
};

Person::Person(string_view name) : impl_(name) {}
Person::~Person() noexcept = default;
inline string Person::get_name() const {
    return impl_->getName(*this);
}
inline void Person::greet() {
    cout << "Hello, " << impl_->getName(*this) << "!" << endl;
}

void test() {
    Person p{"world"};
    p.greet();
}

int main() {
    test();
    return 0;
}