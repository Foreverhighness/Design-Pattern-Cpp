#include <iostream>
#include <memory>
#include <optional>
#include <vector>
using namespace std;

// class Profile;
using Profile = string;
struct ProfileIterator;
// 集合接口必须声明一个用于生成迭代器的工厂方法。如果程序中有不同类型的迭
// 代器，你也可以声明多个方法。
struct SocialNetwork {
  virtual ~SocialNetwork() = default;
  virtual unique_ptr<ProfileIterator> createFriendsIterator(string_view profileId) = 0;
  virtual unique_ptr<ProfileIterator> createCoworkersIterator(string_view profileId) = 0;
};

class WeChat : public SocialNetwork {
 public:
  vector<Profile> socialGraphRequest(string_view profileId, string_view type) {
    if (type == "friends") {
      return {"f1", "f2", "f3", "f4", "f5"};
    } else if (type == "coworkers") {
      return {"w1", "w2", "w3", "w4"};
    } else {
      return {};
    }
  }
  unique_ptr<ProfileIterator> createFriendsIterator(string_view profileId) override;
  unique_ptr<ProfileIterator> createCoworkersIterator(string_view profileId) override;
};

struct ProfileIterator {
  virtual ~ProfileIterator() = default;
  virtual Profile getNext() = 0;
  virtual bool hasMore() = 0;
};

// 具体迭代器类。
class WeChatIterator : public ProfileIterator {
 private:
  // 迭代器需要一个指向其遍历集合的引用。
  WeChat& wechat_;
  string profileId_, type_;
  // 迭代器对象会独立于其他迭代器来对集合进行遍历。因此它必须保存迭代器
  // 的状态。
  vector<Profile> cache_;
  typename decltype(cache_)::size_type currentPosition;
  void lazyInit() {
    if (cache_.empty())
      cache_ = wechat_.socialGraphRequest(profileId_, type_);
  //   for (string_view s: cache_) {
  //     cout << s << endl;
  //   }
  }

 public:
  WeChatIterator(WeChat& wechat, string_view profileId, string_view type) : wechat_(wechat), profileId_(profileId), type_(type), cache_(), currentPosition(0) {}
  // 每个具体迭代器类都会自行实现通用迭代器接口。
  Profile getNext() override {
    if (hasMore()) {
      auto ret = cache_[currentPosition];
      ++currentPosition;
      return ret;
    }
    return "";
  }
  bool hasMore() override {
    lazyInit();
    return currentPosition < cache_.size();
  }
};

unique_ptr<ProfileIterator> WeChat::createFriendsIterator(string_view profileId) { return make_unique<WeChatIterator>(*this, profileId, "friends"); }
unique_ptr<ProfileIterator> WeChat::createCoworkersIterator(string_view profileId) { return make_unique<WeChatIterator>(*this, profileId, "coworkers"); }
// 这里还有一个有用的绝招：你可将迭代器传递给客户端类，无需让其拥有访问整
// 个集合的权限。这样一来，你就无需将集合暴露给客户端了。
//
// 还有另一个好处：你可在运行时将不同的迭代器传递给客户端，从而改变客户端
// 与集合互动的方式。这一方法可行的原因是客户端代码并没有和具体迭代器类相
// 耦合。
class SocialSpammer {
 public:
  static void send(ProfileIterator* iterator, string_view message) {
    while (iterator->hasMore()) {
      auto profile = iterator->getNext();
      cout << "Send " << message << " to " << profile << endl;
    }
  }
};
struct App {
  App() {
    auto network = WeChat();
    auto fit = network.createFriendsIterator("id1");
    auto wit = network.createCoworkersIterator("id2");
    SocialSpammer::send(fit.get(), "hello friend");
    SocialSpammer::send(wit.get(), "hello coworker");
  }
};

int main() {
  App();
  return 0;
}
