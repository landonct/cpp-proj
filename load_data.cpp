#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <ctime>
#include <string_view>
#include <iomanip>
#include <variant>
#include <sstream>

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

    std::variant<std::string, std::time_t, double, int>
    get(int col)
    {
        switch (col)
        {
        case 0:
            return symbol;
        case 1:
            return timestamp;
        case 2:
            return open;
        case 3:
            return volume;
        default:
            std::runtime_error("Column index not found");
        }
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

    Table() = default;

    Table(std::vector<Row> &row_vec)
    {
        for (const Row row : row_vec)
        {
            table.push_back(row);
        }
    }

    void print() const
    {
        for (Row row : table)
        {
            row.print();
        }
    }

    void print_raw() const
    {
        for (Row row : table)
        {
            std::string str_time = std::ctime(&row.timestamp);
            str_time.pop_back();
            std::cout << row.symbol << ", "
                      << str_time << ", "
                      << row.open << ", "
                      << row.volume << "\n";
        }

        std::cout << "\n";
    }

    Table add(Row new_row)
    {
        std::vector<Row> old_table;

        for (Row &row : table)
        {
            old_table.push_back(row);
        }

        old_table.push_back(new_row);
        Table table_added = old_table;

        return table_added;
    }

    int count()
    {
        int count = 0;
        for (Row &row : table)
        {
            count++;
        }

        return count;
    }

    double sum_open()
    {
        double sum = 0;
        for (Row row : table)
        {
            sum += row.open;
        }

        return sum;
    }

    double sum_volume()
    {
        double volume = 0;
        for (Row row : table)
        {
            volume += row.volume;
        }

        return volume;
    }

    Table head(int n = 5)
    {
        Table table_head;
        int i = 0;
        for (Row row : table)
        {
            table_head.add(row);
            i++;

            if (i >= n)
            {
                break;
            }
        }

        table_head.print_raw();
        return table;
    }

    Table filter(std::string filter_expression)
    {
        Table table;
        return table;
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

std::vector<Row> read_csv(std::ifstream &input, int max_rows = 1000000)
{
    std::string strInput{};
    std::vector<std::string> names{};
    std::vector<std::string> strInputVec{};
    int i{0};
    std::vector<Row> table;

    while (std::getline(input, strInput) && i <= max_rows)
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
            i++;
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

    std::vector<Row> row_table = read_csv(input, 2);
    Table table = row_table;

    for (int i = 0; i <= 1; i++)
    {
        row_table[i].print();
    }

    std::cout << table.count() << "\n";
    std::cout << table.sum_open() << "\n";
    std::cout << table.sum_volume() << "\n";

    table.print_raw();
    table.head();

    return 0;
}
