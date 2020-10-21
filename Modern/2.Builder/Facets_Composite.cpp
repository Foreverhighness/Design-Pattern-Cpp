#include <iostream>
#include <string>
using namespace std;
class PersonBuilder;
class Person {
    string street_address, post_code, city;
    string company_name, position;
    int annual_income = 0;

   public:
    static PersonBuilder create();
    friend std::ostream& operator<<(std::ostream& os, const Person& obj) {
        return os << "street_address: " << obj.street_address << " post_code: " << obj.post_code
                  << " city: " << obj.city << " company_name: " << obj.company_name << " position: " << obj.position
                  << " annual_income: " << obj.annual_income;
    }
    friend class PersonBuilder;
    friend class PersonAddressBuilder;
    friend class PersonJobBuilder;
};

class PersonAddressBuilder;
class PersonJobBuilder;
class PersonBuilderBase {
   protected:
    Person& person;
    explicit PersonBuilderBase(Person& person) : person(person) {}

   public:
    operator Person() const { return std::move(person); }
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;
};
class PersonBuilder : public PersonBuilderBase {
    Person p;

   public:
    PersonBuilder() : PersonBuilderBase(p) {}
};
PersonBuilder Person::create() {
    return {};
}

class PersonAddressBuilder : public PersonBuilderBase {
    using Self = PersonAddressBuilder;
   public:
    explicit PersonAddressBuilder(Person& p) : PersonBuilderBase(p) {}
    Self& at(string_view street_address) {
        person.street_address = street_address;
        return *this;
    }
    Self& with_postcode(string_view post_code) {
        person.post_code = post_code;
        return *this;
    }
    Self& in(string_view city) {
        person.city = city;
        return *this;
    }
};
class PersonJobBuilder : public PersonBuilderBase {
    using Self = PersonJobBuilder;
   public:
    explicit PersonJobBuilder(Person& p) : PersonBuilderBase(p) {}
    Self& at(string_view company_name) {
        person.company_name = company_name;
        return *this;
    }
    Self& as_a(string_view position) {
        person.position = position;
        return *this;
    }
    Self& earning(int annual_income) {
        person.annual_income = annual_income;
        return *this;
    }
};

PersonAddressBuilder PersonBuilderBase::lives() const {
    return PersonAddressBuilder(person);
}
PersonJobBuilder PersonBuilderBase::works() const {
    return PersonJobBuilder(person);
}

void test() {
    Person p = Person::create()
        .lives()
            .at("123 London Road")
            .with_postcode("SW1 1GB")
            .in("London")
        .works()
            .at("PragmaSoft")
            .as_a("Consultant")
            .earning(10e6);
    cout << p << endl;
}

int main() {
    test();
    return 0;
}