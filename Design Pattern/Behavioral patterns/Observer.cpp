#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
using namespace std;
// 这里是订阅者接口。
struct EventListener {
  virtual ~EventListener() = default;
  virtual void update(string_view message) = 0;
};
// 具体订阅者会对其注册的发布者所发出的更新消息做出响应。
class LoggingListener : public EventListener {
 public:
  LoggingListener(string_view log_header) : log{log_header} {}
  void update(string_view message) override {
    cout << "log: " << message << "\n";
    log += "\n" + static_cast<string>(message);
  }

  string log;
};

class EmailAlertsListener : public EventListener {
 public:
  EmailAlertsListener(string_view email_addr) : email{email_addr} {}
  void update(string_view message) override { cout << "Send to " << email << ": " << message << "\n"; }

 private:
  string email;
};

// 发布者基类包含订阅管理代码和通知方法。
class EventManager {
 public:
  void subscribe(string_view eventType, const shared_ptr<EventListener>& listener) {
    listeners[static_cast<string>(eventType)].emplace_back(listener);
  }
  void unsubscribe(string_view eventType, const shared_ptr<EventListener>& listener) {
    auto& vec = listeners[static_cast<string>(eventType)];
    auto it = find_if(begin(vec), end(vec), [&listener](auto&& elem) {
      auto sp = elem.lock();
      return sp && sp == listener;
    });
    if (it != end(vec))
      *it = weak_ptr<EventListener>();
  }
  void notify(string_view eventType, string_view message) {
    auto& vec = listeners[static_cast<string>(eventType)];
    for (auto&& wp : vec)
      if (auto sp = wp.lock(); sp)
        sp->update(message);
  }

 private:
  map<string, vector<weak_ptr<EventListener>>> listeners;
};

// 具体发布者包含一些订阅者感兴趣的实际业务逻辑。我们可以从发布者基类中扩
// 展出该类，但在实际情况下并不总能做到，因为具体发布者可能已经是子类了。
// 在这种情况下，你可用组合来修补订阅逻辑，就像我们在这里做的一样。
class Editor {
 public:
  EventManager events;

  void log(string_view message) { events.notify("log", message); }
  void email(string_view message) { events.notify("email", message); }
};

struct App {
  App() {
    auto editor = make_unique<Editor>();
    shared_ptr<EventListener> logger = make_shared<LoggingListener>("logger");
    shared_ptr<EventListener> email_alerts = make_shared<EmailAlertsListener>("123@abc.com");
    editor->events.subscribe("log", logger);
    editor->events.subscribe("email", email_alerts);

    editor->log("first log.");
    editor->email("first email.");

    editor->events.unsubscribe("log", logger);
    editor->events.unsubscribe("log", logger);
    editor->events.unsubscribe("email", email_alerts);
    editor->events.unsubscribe("email", email_alerts);

    editor->log("second log.");
    editor->email("second email.");

    cout << static_cast<LoggingListener*>(logger.get())->log << endl;
  }
};

int main() {
  App();
  return 0;
}
