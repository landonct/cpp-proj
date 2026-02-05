#include <iostream>
#include <vector>
#include <chrono>
#include <string>

struct Row
{
    std::string symbol {};
    std::time_t timestamp {}; // Use <chrono> instead
    double open {};
    // double high {};
    // double low {};
    // double close {};
    int volume {}; 
    // int trade_index {};

    void print() { // Add parameter to change where print output goes
        std::cout << "Symbol: " << symbol << "\n" << 
        "Price opened at " << open << " at exactly " <<
        std::chrono::system_clock::time_point(std::chrono::seconds(timestamp)) << ". Total volume was " << volume << "\n";
    }
};

int main()
{
    Row row1 {"AAPL", 1770093451, 270.1, 100};
    std::vector<Row> vec {};

    row1.print();

    return 0;
}
