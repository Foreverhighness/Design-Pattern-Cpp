// https://zhuanlan.zhihu.com/p/55901945
// https://www.zhihu.com/question/24301047
#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
// memory_order_relaxed
constexpr int N = 1e2;
void test1() {
    std::atomic<int> counter{0};
    std::vector<std::thread> vt;
    for (int i = 0; i < N; ++i) {
        vt.emplace_back([&]() { counter.fetch_add(1, std::memory_order_relaxed); });
    }
    std::for_each(begin(vt), end(vt), [](auto& t) { t.join(); });
    std::cout << "current counter:" << counter << std::endl;
}
// release consume
// std::memory_order_release 的功能如果用一句比较长的话来说，就是：
// 在本行代码之前，如果有任何写内存的操作，都是不能放到本行语句之后的。
// 简单地说，就是写不后。
// 即，写语句不能调到本条语句之后。以这种形式通知编译器/CPU保证真正执行的时候，
// 写语句不会放到 ptr.store(p, std::memory_order_relese) 之后。
//
// std::memory_order_consume 的含义是说，所有后续对本原子类型的操作，必须在本操作完成之后才可以执行。
// 简单点就是不得前。
// 但是这个操作只能用来对读者进行优化。也就是说 release 线程是不能使用这个，只能对读依赖的一方进行优化。
void test2() {
    std::atomic<int*> ptr{nullptr};
    int val = 0;
    std::thread producer{[&]() {
        auto p = new int(42);
        val = 1024;
        ptr.store(p, std::memory_order_release);
    }};
    std::thread consumer{[&]() {
        int* p;
        for (; !(p = ptr.load(std::memory_order_consume));) {
        }
        std::cout << "p: " << *p << std::endl;
        std::cout << "v: " << val << std::endl;
    }};
    producer.join();
    consumer.join();
    delete ptr;
}
// std::memory_order_acquire 表示的是，后续的读操作都不能放到这条指令之前。
// 简单地可以写成读不前。
// memory_order_acq_rel
void test3() {
    std::vector<int> vec;
    std::atomic<int> flag{0};
    std::thread release{[&]() {
        vec.push_back(42);
        flag.store(1, std::memory_order_release);
    }};
    std::thread acqrel{[&]() {
        for (int expected = 1; !flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel); expected = 1) {
        }
        // flag has changed to 2
    }};
    std::thread acquire{[&]() {
        for (; flag.load(std::memory_order_acquire) < 2;) {
        }

        std::cout << vec.at(0) << std::endl;  // must be 42
    }};
    release.join();
    acqrel.join();
    acquire.join();
}
// memory_order_seq_cst
void test4() {
    std::atomic<int> counter{0};
    std::vector<std::thread> vt;
    for (int i = 0; i < N; ++i) {
        vt.emplace_back([&]() { counter.fetch_add(1, std::memory_order_seq_cst); });
    }
    std::for_each(begin(vt), end(vt), [](auto& t) { t.join(); });
    std::cout << "current counter:" << counter << std::endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    return 0;
}