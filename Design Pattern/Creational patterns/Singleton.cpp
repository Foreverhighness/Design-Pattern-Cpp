#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
// 数据库类会对`getInstance（获取实例）`方法进行定义以让客户端在程序各处
// 都能访问相同的数据库连接实例。
class Database {
    static Database* _pinstance;
    static std::mutex _mutex;
    std::string _value;
    Database(std::string_view s) : _value(s) {}

   public:
    static Database& GetInstance(std::string_view s) {
        if (nullptr == _pinstance) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_pinstance)
                _pinstance = new Database(s);
        }
        return *_pinstance;
    }
    std::string value() const { return _value; }
    Database(Database&) = delete;
    Database(Database&&) = delete;
    void operator=(const Database&) = delete;
};
Database* Database::_pinstance{nullptr};
std::mutex Database::_mutex;
void ThreadFoo() {
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Database& singleton = Database::GetInstance("FOO");
    std::cout << singleton.value() << "\n";
}

void ThreadBar() {}
struct App {
    App() {
        std::cout << "If you see the same value, then singleton was reused (yay!\n"
                  << "If you see different values, then more singletons were created (booo!!)\n\n"
                  << "RESULT:\n";
        std::vector<std::string> paras{"Foo", "Bar"};
        std::vector<std::thread> threads;
        std::for_each(paras.begin(), paras.end(), [&threads](std::string_view s) {
            threads.emplace_back([s]() {
                // Following code emulates slow initialization.
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                Database& singleton = Database::GetInstance(s);
                std::cout << singleton.value() << "\n";
            });
        });
        std::for_each(threads.begin(), threads.end(), [](std::thread& t) { t.join(); });
        // for (auto&& t : threads)
        //     t.join();
    }
};
int main() {
    App();
    return 0;
}