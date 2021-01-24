#include <iostream>
using namespace std;

class LightSwitch;
class State {
 public:
  virtual ~State() = default;
  virtual void on(LightSwitch* ls) { cout << "Light is already on\n"; }
  virtual void off(LightSwitch* ls) { cout << "Light is already off\n"; }
};

class OnState : public State {
 public:
  OnState() { cout << "Light turned on\n"; }
  void off(LightSwitch* ls) override;
};
class OffState : public State {
 public:
  OffState() { cout << "Light turned off\n"; }
  void on(LightSwitch* ls) override;
};

class LightSwitch {
 public:
  LightSwitch() : state_{new OffState()} {}
  ~LightSwitch() { delete state_; }
  void set_state(State* new_state) { state_ = new_state; }
  void on() { state_->on(this); }
  void off() { state_->off(this); }

 private:
  State* state_;
};

void test() {
  LightSwitch ls;
  ls.on();
  ls.on();
  ls.off();
  ls.off();
  ls.on();
}

int main() {
  test();
  return 0;
}

inline void OnState::off(LightSwitch* ls) {
  cout << "Switching light off...\n";
  ls->set_state(new OffState());
  delete this;
}
inline void OffState::on(LightSwitch* ls) {
  cout << "Switching light on...\n";
  ls->set_state(new OnState());
  delete this;
}