#include <cpprest/http_client.h>
#include <iostream>
using namespace std;

struct Pingable {
  virtual ~Pingable() = default;
  virtual string ping(string_view message) = 0;
};

class Pong : public Pingable {
 public:
  string ping(string_view message) override {
    string ret{message};
    return ret + " pong";
  }
};

void tryit(Pingable& pp) { cout << pp.ping("ping") << "\n"; }

class RemotePong : public Pingable {
 public:
  string ping(string_view message) override {
    using namespace web;
    using namespace web::http;
    using namespace web::http::client;

    string result;
    http_client client(U("http://localhost:9149/"));
    uri_builder builder(U("/api/pingpong/"));
    builder.append(message.data());
    pplx::task<string> task =
        client.request(methods::GET, builder.to_string()).then([=](http_response r) { return r.extract_string(true); });
    task.wait();
    return task.get();
  }
};

void test() {
  constexpr int n = 3;
  Pong pp;
  for (int i{0}; i < n; ++i)
    tryit(pp);

  RemotePong rp;
  for (int i{0}; i < n; ++i)
    tryit(rp);
}

int main() {
  test();
  return 0;
}