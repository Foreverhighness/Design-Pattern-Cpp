#include <iostream>
#include <map>
#include <vector>
using namespace std;

enum class State { OffHook, Connecting, Connected, OnHold, OnHook };
enum class Trigger { CallDialed, HungUp, CallConnected, PlacedOnHold, TakenOffHold, LeftMessage, StopUsingPhone };

inline ostream& operator<<(ostream& os, const State& obj) {
  switch (obj) {
    case State::OffHook:
      os << "off the hook";
      break;
    case State::Connecting:
      os << "connecting";
      break;
    case State::Connected:
      os << "connected";
      break;
    case State::OnHold:
      os << "on hold";
      break;
    case State::OnHook:
      os << "on the hook";
      break;
  }
  return os;
}
inline ostream& operator<<(ostream& os, const Trigger& obj) {
  switch (obj) {
    case Trigger::CallDialed:
      os << "call dialed";
      break;
    case Trigger::HungUp:
      os << "hung up";
      break;
    case Trigger::CallConnected:
      os << "call connected";
      break;
    case Trigger::PlacedOnHold:
      os << "placed on hold";
      break;
    case Trigger::TakenOffHold:
      os << "taken off hold";
      break;
    case Trigger::LeftMessage:
      os << "left message";
      break;
    case Trigger::StopUsingPhone:
      os << "putting phone on hook";
      break;
    default:
      break;
  }
  return os;
}

auto init() {
  map<State, vector<pair<Trigger, State>>> rules;
  rules[State::OffHook] = {
      {Trigger::CallDialed, State::Connecting},
      {Trigger::StopUsingPhone, State::OnHook},
  };
  rules[State::OffHook] = {
      {Trigger::CallDialed, State::Connecting},
      {Trigger::StopUsingPhone, State::OnHook},
  };
  rules[State::Connecting] = {
      {Trigger::HungUp, State::OffHook},
      {Trigger::CallConnected, State::Connected},
  };
  rules[State::Connected] = {
      {Trigger::LeftMessage, State::OffHook},
      {Trigger::HungUp, State::OffHook},
      {Trigger::PlacedOnHold, State::OnHold},
  };
  rules[State::OnHold] = {
      {Trigger::TakenOffHold, State::Connected},
      {Trigger::HungUp, State::OffHook},
  };
  return rules;
}

void test() {
  auto rules = init();
  State current_state{State::OffHook};
  const State exit_state{State::OnHook};

  for (;;) {
    cout << "The phone is currently " << current_state << endl;
  select_trigger:
    cout << "Select a trigger:\n";
    int i = 0;
    auto& rule = rules[current_state];
    for (auto [trigger_info, _] : rule) {
      cout << i++ << "." << trigger_info << "\n";
    }
    int input;
    cin >> input;
    if (!(0 <= input && input < rule.size())) {
      cout << "Incorrect option, please try again.\n";
      goto select_trigger;
    }

    current_state = rules[current_state].at(input).second;
    if (current_state == exit_state)
      break;
  }
  cout << "We are done using the phone" << endl;
}

int main() {
  test();
  return 0;
}
