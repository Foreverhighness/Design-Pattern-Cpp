#include <iostream>
#include <memory>
using namespace std;
// boost libray is not compatible.
// #include <boost/archive/text_iarchive.hpp>
// #include <boost/archive/text_oarchive.hpp>
// #include <boost/serialization/serialization.hpp>

struct Address {
    string street, city;
    int suite;
    Address(){};
    Address(string_view street, string_view city, int suite) : street(street), city(city), suite(suite) {}
    friend ostream& operator<<(ostream& os, const Address& obj) {
        return os << "street: " << obj.street << " city: " << obj.city << " suite: " << obj.suite;
    }

//    private:
//     friend class boost::serialization::access;
//     template <typename Archive>
//     void serialize(Archive& ar, const unsigned version) {
//         ar& street;
//         ar& city;
//         ar& suite;
//         // ar& street& city& suite;
//     }
};

struct Contact {
    string name;
    shared_ptr<Address> address;
    Contact(){};
    Contact(string_view name, shared_ptr<Address>& address) : name(name), address(address) {}
    friend ostream& operator<<(ostream& os, const Contact& obj) {
        return os << "name: " << obj.name << " works at " << *obj.address;
    }

//    private:
//     friend class boost::serialization::access;
//     template <typename Archive>
//     void serialize(Archive& ar, const unsigned version) {
//         ar& name;
//     }
};

int main() {
    // auto clone = [](const Contact& c) {
    //     using text_oarchive = boost::archive::text_oarchive;
    //     using text_iarchive = boost::archive::text_iarchive;
    //     ostringstream oss;
    //     text_oarchive oa(oss);
    //     oa << c;

    //     string c_serialized{oss.str()};
    //     // cout << c_serialized << endl;

    //     // istringstream iss;
    //     // text_iarchive ia(iss);
    //     Contact ret;
    //     // ia >> ret;
    //     return ret;
    // };
    auto addr = make_shared<Address>("123 East Dr", "London", 0);
    // Contact john{"John Doe", addr};
    // john.address->suite = 123;
    Contact jane{"Jane Doe", addr};
    jane.address->suite = 124;

    // Contact jane2{jane};  // shallow copy
    // Contact jane2{clone(jane)};
    // jane2.address->suite = 555;

    // std::cout << jane2 << std::endl;
    // Contact jane{"Jane Doe", new Address{*addr}};
    // jane.address->suite = 125;

    // cout << john << "\n" << jane << endl;
    return 0;
}