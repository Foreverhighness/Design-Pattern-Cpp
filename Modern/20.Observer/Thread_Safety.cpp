#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

template <typename T>
struct Observer {
  virtual ~Observer() = default;
  virtual void field_changed(T& source, string_view filed_name) = 0;
};

template <typename T>
class Observable {
 public:
  void notify(T& source, string_view name) {
    for (auto& wp : observers)
      if (auto sp = wp.lock(); sp)
        sp->field_changed(source, name);
  }
  void subscribe(shared_ptr<Observer<T>>& spo) { observers.emplace_back(spo); }
  void unsubscribe(shared_ptr<Observer<T>>& spo) {
    auto it = find_if(cbegin(observers), cend(observers), [&spo](auto& wp) {
      auto sp = wp.lock();
      return sp && sp == spo;
    });
    if (it != cend(observers)) {
      this_thread::sleep_for(100ms);  // it cause dangling pointer
      observers.erase(it);
    }
  }

 private:
  vector<weak_ptr<Observer<T>>> observers;
};

template <typename T>
class ThreadSafeObservable : public Observable<T> {
 public:
  void notify(T& source, string_view name) {
    scoped_lock lock{mu};
    Observable<T>::notify(source, name);
  }
  void subscribe(shared_ptr<Observer<T>>& spo) {
    scoped_lock lock{mu};
    Observable<T>::subscribe(spo);
  }
  void unsubscribe(shared_ptr<Observer<T>>& spo) {
    scoped_lock lock{mu};
    Observable<T>::unsubscribe(spo);
  }

 private:
  using mutex_t = std::mutex;
  mutex_t mu;
};

// class Person : public Observable<Person> { // buggy
class Person : public ThreadSafeObservable<Person> {
 public:
  int get_age() const { return age_; }
  void set_age(const int age) {
    if (age_ == age)
      return;
    age_ = age;
    notify(*this, "age");
  }
  Person(const int age) : age_{age} {}

 private:
  int age_{0};
};

class ConsolePersonObserver : public Observer<Person> {
 private:
  void field_changed(Person& source, string_view filed_name) override {
    cout << "Person's " << filed_name << " has changed to ";
    if (filed_name == "age")
      cout << source.get_age();
    cout << ".\n";
  }
};

void test() {
  Person p{20};
  shared_ptr<Observer<Person>> sp = make_shared<ConsolePersonObserver>();
  p.subscribe(sp);
  p.set_age(21);  // Person's age has changed to 21.
  p.set_age(22);  // Person's age has changed to 22.
  thread t1{[&p, &sp]() {
    try {
      p.unsubscribe(sp);
    } catch (const std::exception& e) { cout << e.what() << endl; }
  }};
  thread t2{[&p, &sp]() {
    try {
      p.unsubscribe(sp);
    } catch (const std::exception& e) { cout << e.what() << endl; }
  }};
  t1.join();
  t2.join();
  p.set_age(23);  //
}

int main() {
  test();
  return 0;
}
