#include <iostream>
#include <vector>
using namespace std;

#include <boost/type_index.hpp>

// back-end
#include <boost/msm/back/state_machine.hpp>

// front-end
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;

// transitions/events
struct CallDialed {};
struct HungUp {};
struct CallConnected {};
struct PlacedOnHold {};
struct TakenOffHold {};
struct LeftMessage {};
struct PhoneThrownIntoWall {};

struct PhoneStateMachine : public state_machine_def<PhoneStateMachine> {
  inline static const vector<string> state_names{"off hook"s, "connecting"s, "connected"s, "on hold"s, "destroyed"s};
  bool angry{false};  // start with false
  struct OffHook : state<> {};
  struct Connecting : state<> {
    template <class Event, class FSM>
    void on_entry(Event const& evt, FSM&) {
      cout << "We are connecting..." << endl;
    }
    // also on_exit
  };
  struct Connected : state<> {};
  struct OnHold : state<> {};
  struct PhoneDestroyed : state<> {};

  struct PhoneBeingDestroyed {
    template <class Event, class FSM, class SourceState, class TargetState>
    void operator()(Event const&, FSM&, SourceState&, TargetState&) {
      cout << "Phone breaks into a million pieces" << endl;
    }
  };

  struct CanDestroyPhone {
    template <class Event, class FSM, class SourceState, class TargetState>
    bool operator()(Event const&, FSM& fsm, SourceState&, TargetState&) {
      return fsm.angry;
    }
  };

  // start, event, target, action, guard
  struct transition_table
      : mpl::vector<Row<OffHook, CallDialed, Connecting>,
                    Row<Connecting, CallConnected, Connected>,
                    Row<Connected, PlacedOnHold, OnHold>,
                    Row<OnHold, PhoneThrownIntoWall, PhoneDestroyed, PhoneBeingDestroyed, CanDestroyPhone> > {};

  // starting state
  typedef OffHook initial_state;

  // what happens if there's nowhere to go
  template <class FSM, class Event>
  void no_transition(Event const& e, FSM&, int state) {
    using boost::typeindex::type_id;
    cout << "No transition from state [" << state_names[state] << "] on event [" << type_id<decltype(e)>().pretty_name()
         << "]\n";
  };
};

void test() {
  msm::back::state_machine<PhoneStateMachine> phone;
  auto info = [&phone]() {
    auto i = phone.current_state()[0];
    cout << "The phone is currently [" << PhoneStateMachine::state_names[i] << "]\n";
  };

  info();
  phone.process_event(CallDialed{});
  info();
  phone.process_event(CallConnected{});
  info();
  phone.process_event(PlacedOnHold{});
  info();
  phone.process_event(PhoneThrownIntoWall{});
  info();

  phone.angry = true;
  phone.process_event(PhoneThrownIntoWall{});
  info();

  // try process_event here :)
  phone.process_event(CallDialed{});

  cout << "We are done using the phone\n";
}

int main() {
  test();
  return 0;
}
