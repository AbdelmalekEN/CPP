#include "BitcoinExchange.hpp"

#include <iostream>

int main(int ac, char **av)
{
    if (ac != 2)
        return (std::cout << "invalid args" << std::endl, 1);
    try
    {
        BitcoinExchange coin;

        // coin.get_data("data.csv");
        coin.get_data("simple_data.csv");
        coin.parse_input(av[1]);
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }
}