#include <iostream>
#include <memory>
#include <vector>
// #include <algorithm>
using namespace std;

vector<int> global_data{1, 2, 3};
void printVec(const vector<int>& data) {
    cout << "[";
    for (auto i = 0; i < data.size(); ++i) {
        if (i != 0)
            cout << ", ";
        cout << data[i];
    }
    cout << "]\n";
}
// 装饰可以改变组件接口所定义的操作。
struct DataSource {
    virtual void writeData(vector<int>& data) = 0;
    virtual vector<int> readData() = 0;
    virtual ~DataSource() = default;
};

class FileDataSource : public DataSource {
   protected:
    string filename_;

   public:
    FileDataSource(string_view name) : filename_(name) {}
    ~FileDataSource() = default;
    void writeData(vector<int>& data) override {
        cout << "Write into " << filename_ << ".\n";
        printVec(data);
        global_data.swap(data);
    }
    vector<int> readData() override {
        cout << "Read from " << filename_ << ".\n";
        return {global_data};
    }
};

// 装饰基类和其他组件遵循相同的接口。该类的主要任务是定义所有具体装饰的封
// 装接口。封装的默认实现代码中可能会包含一个保存被封装组件的成员变量，并
// 且负责对其进行初始化。
struct DataSourceDecorator : public DataSource {
   protected:
    unique_ptr<DataSource> wrappee;

   public:
    DataSourceDecorator(unique_ptr<DataSource> ds) : wrappee(std::move(ds)) {}

    void writeData(vector<int>& data) override { wrappee->writeData(data); }
    vector<int> readData() override { return wrappee->readData(); }
};

// 具体装饰必须在被封装对象上调用方法，不过也可以自行在结果中添加一些内容。
// 装饰必须在调用封装对象之前或之后执行额外的行为。
class EncryptionDecorator : public DataSourceDecorator {
   public:
    EncryptionDecorator(unique_ptr<DataSource> ds) : DataSourceDecorator(std::move(ds)) {}
    void writeData(vector<int>& data) override {
        cout << "EncryptionDecorator writeData by take opposite.\n";
        for (auto&& x : data)
            x = -x;
        wrappee->writeData(data);
    }

    vector<int> readData() override {
        cout << "EncryptionDecorator readData by take opposite.\n";
        auto ret = wrappee->readData();
        for (auto&& x : ret)
            x = -x;
        return ret;
    }
};

class CompressionDecorator : public DataSourceDecorator {
   public:
    CompressionDecorator(unique_ptr<DataSource> ds) : DataSourceDecorator(std::move(ds)) {}
    void writeData(vector<int>& data) override {
        cout << "CompressionDecorator decide to writeData with times 2.\n";
        for (auto&& x : data)
            x *= 2;
        wrappee->writeData(data);
    }

    vector<int> readData() override {
        auto ret = wrappee->readData();
        cout << "CompressionDecorator readData by divided by 2.\n";
        for (auto&& x : ret)
            x /= 2;
        return ret;
    }
};

struct App {
    App() {
        vector salaryRecords{1, 2, 3};

        unique_ptr<DataSource> source{make_unique<FileDataSource>("somefile.dat")};
        source->writeData(salaryRecords);  // 已将明码数据写入目标文件。
        printVec(source->readData());

        source = make_unique<CompressionDecorator>(std::move(source));
        source->writeData(salaryRecords);  // 已将压缩数据写入目标文件。
        printVec(source->readData());

        source = make_unique<EncryptionDecorator>(std::move(source));
        // 源变量中现在包含：
        // Encryption > Compression > FileDataSource
        source->writeData(salaryRecords);  // 已将压缩且加密的数据写入目标文件。
        printVec(source->readData());
    }
};
int main() {
    App();
    return 0;
}
