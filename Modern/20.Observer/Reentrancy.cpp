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
  virtual void field_changed(T& source, string_view field_name) = 0;
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
    auto it = find_if(begin(observers), end(observers), [&spo](auto& wp) {
      auto sp = wp.lock();
      return sp && sp == spo;
    });
    if (it != end(observers)) {
      this_thread::sleep_for(50ms);  // it cause dangling pointer
      // observers.erase(it);
      *it = weak_ptr<Observer<T>>();  // null object pattern
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
    // scoped_lock lock{mu};
    Observable<T>::unsubscribe(spo);
  }

 private:
  mutex mu;
  // recursive_mutex mu;
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
  void field_changed(Person& source, string_view field_name) override {
    cout << "Person's " << field_name << " has changed to ";
    if (field_name == "age")
      cout << source.get_age();
    cout << ".\n";
  }
};

class TrafficAdministrator : public Observer<Person>, public enable_shared_from_this<TrafficAdministrator> {
 private:
  void field_changed(Person& source, string_view field_name) override {
    if (field_name != "age")
      return;
    cout << source.get_age() << endl;
    if (source.get_age() <= 16) {
      cout << "Whoa there, you are not old enough to drive!\n";
    } else {
      // oh, ok they are old enough, let's not monitor them anymore
      cout << "We no loger care!\n";
      auto sp = static_cast<shared_ptr<Observer<Person>>>(shared_from_this());
      source.unsubscribe(sp);
    }
  }
};

void test() {
  int age = 14;
  Person p{age};
  shared_ptr<Observer<Person>> sp = make_shared<TrafficAdministrator>();
  p.subscribe(sp);
  p.set_age(++age);  // Person's age has changed to 15.
  p.set_age(++age);  // Person's age has changed to 16.
  p.set_age(++age);  // Person's age has changed to 17.
  thread t1{[&p, &sp]() { p.unsubscribe(sp); }};
  thread t2{[&p, &sp]() { p.unsubscribe(sp); }};
  t1.join();
  t2.join();
  p.set_age(++age);  //
}

int main() {
  test();
  return 0;
}
