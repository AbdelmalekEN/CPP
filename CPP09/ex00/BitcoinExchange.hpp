#pragma once

#include <map>
#include <string>

class BitcoinExchange
{
    private:
        std::map<std::string, float> data;

        bool validDate(const std::string &date);
        bool validValue(const std::string &strValue, float &value);
        std::string get_closest_date(const std::string &target);
    public:
        BitcoinExchange();
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);

        void get_data(const std::string &file);
        void parse_input(const std::string &filen);
};