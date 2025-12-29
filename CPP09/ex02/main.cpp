#include "PmergeMe.hpp"
#include <iostream>

int main(int ac, char **av)
{
    if (ac < 2)
        return (std::cout << "invalid args" << std::endl, 1);
    try
    {
        PmergeMe sorter;
        sorter.parse(ac, av);
        sorter.sort();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}