#include <iostream>
using namespace std;
class Printer {
    static int id_;

   public:
    int get() const { return id_; }
    void set(int val) const { id_ = val; }
};
int Printer::id_{-1};
void test() {
    Printer a, b, c;
    cout << a.get() << " " << b.get() << " " << c.get() << endl;
    a.set(4);
    cout << a.get() << " " << b.get() << " " << c.get() << endl;
}

int main() {
    test();
    return 0;
}