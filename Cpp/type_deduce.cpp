#include <array>
#include <boost/type_index.hpp>
#include <iostream>
#include <vector>

template <typename T>
void show(const T& para) {
  using boost::typeindex::type_id_with_cvr;
  using std::cout;

  // show T
  cout << "T    = " << type_id_with_cvr<T>().pretty_name() << "\n";

  // show para's type
  cout << "para = " << type_id_with_cvr<decltype(para)>().pretty_name() << "\n";
}

class Widget {};

std::vector<Widget> createVec() { return {{}, {}}; }

int main() {
  // const auto vec = createVec();
  // if (!vec.empty()) {
  //   show(&vec[0]);
  //   show(vec[0]);
  //   show(&vec);
  //   show(vec);
  // }
  std::vector<bool> vb;
  vb.emplace_back();
  if (!vb.empty()) {
    // show(&vb[0]);
    show(vb[0]);
    // show(&vb);
    // show(vb);
  }
  std::array<bool, 1> ab;
  show(ab[0]);

  return 0;
}