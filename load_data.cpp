#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <ctime>
#include <string_view>
#include <iomanip>

struct Row
{
    std::string symbol{};
    std::time_t timestamp{};
    double open{};
    int volume{};

    Row() = default; // Default constructor

    Row(const std::vector<std::string> &fields) // Construct from a vector
    {
        if (fields.size() != 4)
        {
            throw std::runtime_error("Wrong number of values in fields");
        }
        symbol = fields[0];
        timestamp = parse_iso(fields[1]);
        open = std::stod(fields[2]);
        volume = std::stoi(fields[3]);
    }

    Row(std::string sym, std::time_t tsmp, double o, int vol)
        : symbol(sym), // This syntax prevents default initialization to avoid an extra copy being made
          timestamp(tsmp),
          open(o),
          volume(vol)
    {
    }

    void print(std::ostream &out = std::cout) const
    { // Add parameter to change where print output goes
        out << "Symbol: " << symbol << "\n"
            << "Price opened at " << open << " at " << std::ctime(&timestamp) << "Total volume was " << volume << "\n";
    }

    std::time_t parse_iso(const std::string &s)
    {
        std::time_t out_time{};
        std::tm time{}; // Add in a method to parse ISO time string
        std::istringstream ss(s);
        ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S");

        if (ss.fail())
        {
            throw std::runtime_error("Failed to parse: " + s);
        }

#ifdef _WIN32
        return _mkgmtime(&time);
#else
        return timegm(&time);
#endif
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

struct Table
{
    std::vector<Row> table;

    void print() const
    {
        for (Row row : table)
        {
            row.print();
        }
    }

    
};

Row process_row(std::string row_string, char delim = ',')
{
    int max_delim{};
    for (int i = 0; i < row_string.length(); i++)
    {
        if (row_string[i] == delim)
        {
            max_delim++;
        }
    }

    std::vector<std::string> values;
    for (int j = 0; j <= max_delim; j++)
    {
        values.push_back(row_string.substr(0, row_string.find(delim)));
        row_string.erase(0, row_string.find(delim) + 1);
    }
    Row row{values};

    return row;
}

Name process_names(std::string names_string, char delim = ',')
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

std::vector<Row> read_csv(std::ifstream &input)
{
    std::string strInput{};
    std::vector<std::string> names{};
    std::vector<std::string> strInputVec{};
    int i{0};
    std::vector<Row> table;

    while (std::getline(input, strInput))
    {
        if (i == 0)
        {
            Name name = process_names(strInput);
            i++;
        }
        else
        {
            Row row = process_row(strInput);
            table.push_back(row);
        }
    }
    return table;
}

std::time_t parse_iso(const std::string &s)
{
    std::time_t out_time{};
    std::tm time{}; // Add in a method to parse ISO time string
    std::istringstream ss(s);
    ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail())
    {
        throw std::runtime_error("Failed to parse: " + s);
    }

#ifdef _WIN32
    return _mkgmtime(&time);
#else
    return timegm(&time);
#endif
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

    std::ifstream input{"spy_reduced.csv"};
    if (!input)
    {
        std::cerr << "Failed to read file" << "\n";
        return 1;
    }
    std::vector<Row> table = read_csv(input);
    // for (const Row row : table)
    for (int i = 0; i <= 1; i++)
    {
        table[i].print();
    }

    return 0;
}
