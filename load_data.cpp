#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <ctime>

struct Row
{
    std::string symbol {};
    std::time_t timestamp {};
    double open {};
    int volume {}; 

    void print(std::ofstream& out) const { // Add parameter to change where print output goes
        out << "Symbol: " << symbol << "\n" << 
        "Price opened at " << open << " at exactly " <<
        std::ctime(&timestamp) << "Total volume was " << volume << "\n";
    }
};

int main()
{
    time_t now;
    time(&now);
    Row row1 {"AAPL", now, 270.1, 100};
    std::vector<Row> vec {};

    std::ofstream out{ "sample.txt" };
    if(!out) { // !out.is_open() || out.fail() Longer but has more checks
        std::cerr << "Failed to open file" << "\n";
        return 1;
    }
    row1.print(out);

    return 0;
}
