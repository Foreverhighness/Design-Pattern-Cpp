#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct ChatRoom;
struct Person {
  string name;
  ChatRoom* room = nullptr;
  vector<string> chat_log;

  explicit Person(string_view name) : name(name) {}

  void receive(string_view origin, string_view message);
  void say(string_view message) const;
  void pm(string_view who, string_view message) const;
};

struct ChatRoom {
  vector<Person*> people;
  void join(Person* p) {
    string join_msg = p->name + " joins the chat";
    broadcast("room", join_msg);
    p->room = this;
    people.emplace_back(p);
  }
  void broadcast(string_view origin, string_view message) {
    for (auto p : people)
      if (p->name != origin)
        p->receive(origin, message);
  }
  void message(string_view origin, string_view who, string_view message) {
    if (auto it = find_if(begin(people), end(people), [&](const Person* p) { return p->name == who; });
        it != end(people)) {
      (*it)->receive(origin, message);
    }
  }
};

void Person::receive(string_view origin, string_view message) {
  ostringstream oss;
  oss << origin << ": \"" << message << "\"";
  cout << "[" << name << "'s chat session] " << oss.str() << "\n";
  chat_log.emplace_back(oss.str());
}
void Person::say(string_view message) const { room->broadcast(name, message); }
void Person::pm(string_view who, string_view message) const { room->message(name, who, message); }

void test() {
  ChatRoom room;

  Person john{"john"};
  Person jane{"jane"};
  room.join(&john);
  room.join(&jane);
  john.say("hi room");
  jane.say("oh, hey john");

  Person simon("simon");
  room.join(&simon);
  simon.say("hi everyone!");

  jane.pm("simon", "glad you could join us, simon");
}

int main() {
  test();
  return 0;
}