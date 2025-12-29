#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Orthodox Canonical Form
BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        _priceData = other._priceData;
    }
    return *this;
}

// Load CSV price database
void BitcoinExchange::loadPriceData(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open database file.");
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) 
    {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string date;
        std::string priceStr;
        
        if (std::getline(iss, date, ',') && std::getline(iss, priceStr)) 
        {
            std::stringstream priceStream(priceStr);
            float price;
            if (priceStream >> price) {
                _priceData[date] = price;
            }
        }
    }
    file.close();
}

// Check if date is valid (YYYY-MM-DD)
bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    
    // Extract year, month, day
    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());
    
    // Basic range checks
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    
    // Check days in month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Leap year check for February
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29) return false;
        if (!isLeap && day > 28) return false;
    } else if (day > daysInMonth[month - 1]) {
        return false;
    }
    
    return true;
}

// Check if value is valid (0-1000)
bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) {
    // Check if it's a valid number
    char* endptr;
    const char* str = valueStr.c_str();
    float val = strtof(str, &endptr);
    
    if (endptr == str || *endptr != '\0') {
        return false; // Not a valid number
    }
    
    if (val < 0) {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    
    if (val > 1000) {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }
    
    value = val;
    return true;
}

// Find closest earlier date in database
std::string BitcoinExchange::findClosestDate(const std::string& targetDate) {
    if (_priceData.empty()) {
        return "";
    }
    
    // If exact match exists
    if (_priceData.find(targetDate) != _priceData.end()) {
        return targetDate;
    }
    
    // lower_bound returns first element >= target
    std::map<std::string, float>::iterator it = _priceData.lower_bound(targetDate);
    
    // If all dates are greater than target (target before first date)
    if (it == _priceData.begin()) {
        return ""; // No earlier date
    }
    
    // If target is between dates or after last date
    if (it == _priceData.end() || it->first > targetDate) {
        --it; // Move to previous (earlier) date
    }
    
    return it->first;
}

// Process input file
void BitcoinExchange::processInputFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse "date | value"
        size_t delimiter = line.find(" | ");
        if (delimiter == std::string::npos) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string date = line.substr(0, delimiter);
        std::string valueStr = line.substr(delimiter + 3);
        
        // Validate date
        if (!isValidDate(date)) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        // Validate value
        float value;
        if (!isValidValue(valueStr, value)) {
            continue; // Error message already printed in isValidValue
        }
        
        // Find price for date
        std::string closestDate = findClosestDate(date);
        if (closestDate.empty()) {
            std::cout << "Error: no price data available for date " << date << std::endl;
            continue;
        }
        
        float price = _priceData[closestDate];
        float result = value * price;
        
        // Print result
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    
    file.close();
}