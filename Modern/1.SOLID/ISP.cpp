#include <memory>
#include <vector>
using namespace std;
struct Document;
struct IMyMachine {
    virtual void print(vector<Document*>& docs) = 0;
    virtual void fax(vector<Document*>& docs) = 0;
    virtual void scan(vector<Document*>& docs) = 0;
};
struct MyFavoritePrinter : IMyMachine {
    void print(vector<Document*>& docs) override;
    void fax(vector<Document*>& docs) override;
    void scan(vector<Document*>& docs) override;
};

struct IPrinter {
    virtual void print(vector<Document*>& docs) = 0;
};
struct IFaxer {
    virtual void fax(vector<Document*>& docs) = 0;
};
struct IScanner {
    virtual void scan(vector<Document*>& docs) = 0;
};

struct IMachine : IPrinter, IFaxer, IScanner {};
class Machine : public IMachine {
    unique_ptr<IPrinter> _printer;
    unique_ptr<IFaxer> _faxer;
    unique_ptr<IScanner> _scanner;

   public:
    Machine(IPrinter* printer, IFaxer* faxer, IScanner* scanner) : _printer(printer), _faxer(faxer), _scanner(scanner) {}
    void print(vector<Document*>& docs) override { _printer->print(docs); }
    void fax(vector<Document*>& docs) override { _faxer->fax(docs); }
    void scan(vector<Document*>& docs) override { _scanner->scan(docs); }
};

int main() {
    return 0;
}