#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange()	{}
BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		data = other.data;
	return *this;
}

void BitcoinExchange::get_data(const std::string &file)
{
	std::ifstream f(file.c_str());
	if (!f.is_open())
		throw std::runtime_error("Error: couldn't open database");
	
	std::string line;
	std::getline(f, line);
	while (std::getline(f, line))
	{
		if (line.empty())
			continue;

		std::istringstream isstr(line);
		std::string date;
		std::string price;

		if (std::getline(isstr, date, ',') && std::getline(isstr, price))
		{
			std::stringstream priceStr(price);
			float pricef;
			if (priceStr >> pricef)
				data[date] = pricef;
		}

	}
	f.close();
}

bool BitcoinExchange::validDate(const std::string &date)
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7) continue;
		if (!isdigit(date[i])) return false;
	}

	int year = atoi(date.substr(0, 4).c_str());
	int month = atoi(date.substr(5, 2).c_str());
	int day = atoi(date.substr(8, 2).c_str());

	if (year < 1 || month < 1 || day < 1 || month > 12 || day > 31)
		return false;
	
	return true;
}

bool BitcoinExchange::validValue(const std::string &strValue, float &value)
{
	char *endptr;
	const char *str = strValue.c_str();
	float v = strtof(str, &endptr);

	if (endptr == str || *endptr != '\0')
		return false;
	if (v < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	if (v > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}

	value = v;
	return true;
}

std::string BitcoinExchange::get_closest_date(const std::string &target)
{
	if (data.empty())
		return "";
	if (data.find(target) != data.end())
		return target;

	std::map<std::string, float>::iterator iter = data.lower_bound(target);

	if (iter == data.begin())
		return "";
	if (iter == data.end() || iter->first > target)
		--iter;

	return iter->first;
}

void BitcoinExchange::parse_input(const std::string &filen)
{
	std::ifstream file(filen.c_str());
	if(!file.is_open())
	{
		std::cout << "Error: couldn't open file" << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);
	while(std::getline(file, line))
	{
		if (line.empty())
			continue;
		size_t delim = line.find(" | ");
		if (delim == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = line.substr(0, delim);
		std::string valStr = line.substr(delim + 3);

		if (!validDate(date))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		float val;
		if (!validValue(valStr, val))
			continue;
		std::string closestDate = get_closest_date(date);
		if (closestDate.empty())
		{
			std::cout << "Error: no price data found for date " << date << std::endl;
			continue;
		}
		float price = data[closestDate];
		float res = val * price;
		std::cout << date << " => " << val << " = " << res << std::endl;
	}
	file.close();
}