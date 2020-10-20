#include <fstream>
#include <string>
#include <string_view>
#include <vector>
using std::string;
using std::string_view;
using std::vector;

struct Journal {
    string title;
    vector<string> entries;

    explicit Journal(string_view title) : title{title} {}
    Journal& add(string_view);
};

struct PersistenceManager {
    static void save(const Journal& journal, string_view filename) {
        std::ofstream ofs{static_cast<string>(filename)};
        for (auto&& entry : journal.entries)
            ofs << entry << std::endl;
    }
};

Journal& Journal::add(string_view entry) {
    static int count = 1;
    entries.emplace_back(std::to_string(count++) + " " + entry.data());
    return *this;
}

int main() {
    Journal j{"Dear Diary"};
    j.add("I cried today").add("I ate a bug");
    PersistenceManager::save(j, "now");
    return 0;
}