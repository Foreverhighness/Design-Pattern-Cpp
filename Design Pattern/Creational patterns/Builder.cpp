// 只有当产品较为复杂且需要详细配置时，使用生成器模式才有意义。下面的两个
// 产品尽管没有同样的接口，但却相互关联。
#include <iostream>
#include <memory>
#include <string>
using std::string;
using std::unique_ptr;
// 一辆汽车可能配备有 GPS 设备、行车电脑和几个座位。不同型号的汽车（
// 运动型轿车、SUV 和敞篷车）可能会安装或启用不同的功能。
struct Car {
    string seats;
    string engine;
    string tripComputer;
    string GPS;
    void p() { std::cout << "{" << seats << ", " << engine << ", " << tripComputer << ", " << GPS << "}" << std::endl; }
};

// 用户使用手册应该根据汽车配置进行编制，并介绍汽车的所有功能。
struct Manual {
    string seats;
    string engine;
    string tripComputer;
    string GPS;
    void p() { std::cout << "{" << seats << ", " << engine << ", " << tripComputer << ", " << GPS << "}" << std::endl; }
};

// 生成器接口声明了创建产品对象不同部件的方法。
struct Builder {
    virtual ~Builder() {}
    virtual Builder& seats(const string&) = 0;
    virtual Builder& engine(const string&) = 0;
    virtual Builder& tripComputer(const string&) = 0;
    virtual Builder& GPS(const string&) = 0;
};

// 具体生成器类将遵循生成器接口并提供生成步骤的具体实现。你的程序中可能会
// 有多个以不同方式实现的生成器变体。
class CarBuilder : public Builder {
    Car& car;

   public:
    explicit CarBuilder(Car& car) : car{car} {}
    operator Car() { return std::move(car); }

    Builder& seats(const string& seats) override {
        car.seats = seats;
        return *this;
    }
    Builder& engine(const string& engine) override {
        car.engine = engine;
        return *this;
    }
    Builder& tripComputer(const string& tripComputer) override {
        car.tripComputer = tripComputer;
        return *this;
    }
    Builder& GPS(const string& GPS) override {
        car.GPS = GPS;
        return *this;
    }
};

class ManualBuilder : public Builder {
    Manual& manual;

   public:
    explicit ManualBuilder(Manual& manual) : manual{manual} {}
    operator Manual() { return std::move(manual); }

    Builder& seats(const string& seats) override {
        manual.seats = seats;
        return *this;
    }
    Builder& engine(const string& engine) override {
        manual.engine = engine;
        return *this;
    }
    Builder& tripComputer(const string& tripComputer) override {
        manual.tripComputer = tripComputer;
        return *this;
    }
    Builder& GPS(const string& GPS) override {
        manual.GPS = GPS;
        return *this;
    }
};

class Director {
    unique_ptr<Builder> builder;

   public:
    // explicit Director(Builder& builder) : builder{builder} {}
    explicit Director(Car& car) : builder{new CarBuilder(car)} {}
    explicit Director(Manual& manual) : builder{new ManualBuilder(manual)} {}
    void constructSportsCar() { builder->seats("c1").engine("e1").tripComputer("t1").GPS("g1"); }
};

class App {
   public:
    void makeCar() {
        {
            Car car;
            Director director(car);
            director.constructSportsCar();
            car.p();
        };
        {
            Manual manual;
            Director director(manual);
            director.constructSportsCar();
            manual.p();
        };
    }
};

int main() {
    App().makeCar();
    return 0;
}
