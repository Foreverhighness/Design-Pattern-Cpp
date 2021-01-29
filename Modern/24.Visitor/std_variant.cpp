#include <iostream>
#include <variant>
using namespace std;

struct AddressPrinter {
  void operator()(string_view house_name) const { cout << "A house called " << house_name << "\n"; }
  void operator()(const int house_number) const { cout << "House number " << house_number << "\n"; }
};

void test() {
  variant<string, int> house;
  house = 221;
  AddressPrinter ap;
  std::visit(ap, house);
  house = "Montefiore Castle";
  std::visit(
      [](auto&& house_info) {
        using T = std::decay_t<decltype(house_info)>;
        if constexpr (std::is_same_v<T, string>) {
          cout << "A house called " << house_info << "\n";
        } else if constexpr (std::is_integral_v<T>) {
          cout << "House number " << house_info << "\n";
        }
      },
      house);
}

int main() {
  test();
  return 0;
}
