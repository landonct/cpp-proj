#include <iostream>
#include <vector>
#include <ctime>

struct Row
{
    std::string symbol {};
    std::time_t timestamp {};
    double open {};
    // double high {};
    // double low {};
    // double close {};
    int volume {}; 
    // int trade_index {};
};

int main()
{
    Row row1 {"AAPL", 1770093451, 270.1, 100};
    std::vector<std::string> vec {};

    std::cout << row1.symbol;

    return 0;
}
