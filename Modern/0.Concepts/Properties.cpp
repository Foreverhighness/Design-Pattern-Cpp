// use clang.
class Person {
    int age_;

   public:
    int get_age() const { return age_; }
    void set_age(int value) { age_ = value; }
    // __declspec(property(get = get_age, put = set_age)) int age;
};