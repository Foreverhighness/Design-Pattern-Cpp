#include <boost/di.hpp>
#include <iostream>
#include <memory>
using namespace std;
namespace di = boost::di;
struct ILogger {
    virtual ~ILogger() {}
    virtual void log_info(string_view) = 0;
};

class Reporting {
    shared_ptr<ILogger> _logger;

   public:
    Reporting(const shared_ptr<ILogger>& logger) : _logger(logger) {}
    void prepare_report() { _logger->log_info("Preparing the report"); }
};

struct Engine {
    float volume = 5.0f;
    int horse_power = 40;

    friend ostream& operator<<(ostream& os, const Engine& obj) {
        return os << "volume: " << obj.volume << " horse_power: " << obj.horse_power;
    }
};

struct ConsoleLogger : ILogger {
    void log_info(string_view s) override { cout << "LOG: " << s << endl; }
};

class Car {
    unique_ptr<Engine> _engine;
    shared_ptr<ILogger> _logger;

   public:
    Car(unique_ptr<Engine> engine, const shared_ptr<ILogger>& logger) : _engine(std::move(engine)), _logger(logger){};
};

int main() {
    auto injector = di::make_injector(di::bind<ILogger>().to<ConsoleLogger>());
    auto car = injector.create<shared_ptr<Car>>();
    return 0;
}
