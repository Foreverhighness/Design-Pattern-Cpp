#include <iostream>
#include <memory>
using namespace std;

struct Address {
    string street, city;
    int suite;
    Address(string_view street, string_view city, int suite) : street(street), city(city), suite(suite) {}
    friend ostream& operator<<(ostream& os, const Address& obj) {
        return os << "street: " << obj.street << " city: " << obj.city << " suite: " << obj.suite;
    }
};

struct Contact {
    string name;
    shared_ptr<Address> address;
    Contact(string_view name, shared_ptr<Address>& address) : name(name), address(address) {}
    Contact(const Contact& other) : name(other.name), address(make_shared<Address>(*(other.address))) {}
    friend ostream& operator<<(ostream& os, const Contact& obj) {
        return os << "name: " << obj.name << " works at " << *obj.address;
    }
};

int main() {
    auto addr = make_shared<Address>("123 East Dr", "London", 0);
    Contact john{"John Doe", addr};
    john.address->suite = 123;
    Contact jane{"Jane Doe", addr};
    jane.address->suite = 124;

    Contact jane2{jane};  // shallow copy
    jane2.address->suite = 555;

    std::cout << jane2 << std::endl;
    // Contact jane{"Jane Doe", new Address{*addr}};
    jane.address->suite = 125;

    cout << john << "\n" << jane << endl;
    return 0;
}