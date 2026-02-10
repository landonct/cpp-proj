#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <ctime>
#include <string_view>

struct Row
{
    std::string symbol{};
    std::time_t timestamp{};
    double open{};
    int volume{};

    void print(std::ostream &out = std::cout) const
    { // Add parameter to change where print output goes
        out << "Symbol: " << symbol << "\n"
            << "Price opened at " << open << " at exactly " << std::ctime(&timestamp) << "Total volume was " << volume << "\n";
    }
};

struct Name
{
    std::vector<std::string> names;

    void print(std::ostream &out = std::cout) const
    {
        for (auto it = names.begin(); it != names.end(); ++it)
        {
            out << *it << " ";
        }

        out << "\n";
    }
};

std::string process_row(std::string row_string)
{
    return "";
    std::cout << row_string << "\n";
    Row row;

    return "";
}

Name process_names(std::string &names_string, char delim = ',')
{
    int max_delim{};
    for (int i = 0; i < names_string.length(); i++)
    {
        if (names_string[i] == delim)
        {
            max_delim++;
        }
    }

    std::vector<std::string> names;
    for (int j = 0; j <= max_delim; j++)
    {
        names.push_back(names_string.substr(0, names_string.find(delim)));
        names_string.erase(0, names_string.find(delim) + 1);
    }

    Name name{names};
    return name;
}

bool read_csv(std::ifstream &input)
{
    std::string strInput{};
    std::vector<std::string> names{};
    std::vector<std::string> strInputVec{};
    int i{0};

    while (std::getline(input, strInput))
    {
        if (i == 0)
        {
            Name name = process_names(strInput);
            name.print();
            i++;
        }
        else
        {
            process_row(strInput);
        }
    }
    return 1;
}

int main()
{
    time_t now;
    time(&now);
    Row row1{"AAPL", now, 270.1, 100};
    std::vector<Row> vec{};

    std::ofstream out{"sample.txt"};
    if (!out)
    { // !out.is_open() || out.fail() Longer but has more checks
        std::cerr << "Failed to open file" << "\n";
        return 1;
    }
    row1.print(out);

    std::ifstream input{"spy.csv"};
    if (!input)
    {
        std::cerr << "Failed to read file" << "\n";
        return 1;
    }
    read_csv(input);

    return 0;
}
