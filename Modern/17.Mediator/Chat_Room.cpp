#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
using namespace std;

struct ChatRoom;
class Person {
 public:
  string name;
  weak_ptr<ChatRoom> room;
  vector<string> chat_log;

  void receive(string_view origin, string_view message);
  void say(string_view message) const;
  void pm(string_view who, string_view message) const;

  static shared_ptr<Person> create(string_view name) { return shared_ptr<Person>{new Person{name}}; }

 private:
  explicit Person(string_view name) : name(name) {}
};

struct ChatRoom : enable_shared_from_this<ChatRoom> {
  vector<weak_ptr<Person>> people;
  void join(shared_ptr<Person>& p) {
    string join_msg = p->name + " joins the chat";
    broadcast("room", join_msg);
    p->room = this->shared_from_this();
    people.emplace_back(p);
  }
  void broadcast(string_view origin, string_view message) {
    for (auto wp : people)
      if (auto sp = wp.lock(); sp && sp->name != origin)
        sp->receive(origin, message);
  }
  void message(string_view origin, string_view who, string_view message) {
    if (auto it = find_if(begin(people), end(people),
                          [who](weak_ptr<Person>& p) { return !p.expired() && p.lock()->name == who; });
        it != end(people)) {
      it->lock()->receive(origin, message);
    }
  }
};

void Person::receive(string_view origin, string_view message) {
  ostringstream oss;
  oss << origin << R"(: ")" << message << R"(")";
  cout << "[" << name << "'s chat session] " << oss.str() << "\n";
  chat_log.emplace_back(oss.str());
}
void Person::say(string_view message) const { room.lock()->broadcast(name, message); }
void Person::pm(string_view who, string_view message) const { room.lock()->message(name, who, message); }

void test() {
  auto room{make_shared<ChatRoom>()};

  auto john{Person::create("john")};
  auto jane{Person::create("jane")};
  room->join(john);
  room->join(jane);
  john->say("hi room");
  jane->say("oh, hey john");

  auto simon{Person::create("simon")};
  room->join(simon);
  simon->say("hi everyone!");

  jane->pm("simon", "glad you could join us, simon");
}

int main() {
  test();
  return 0;
}