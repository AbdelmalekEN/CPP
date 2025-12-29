#include "RPN.hpp"

#include <iostream>

int main(int ac, char **av)
{
    if (ac != 2)
        return (std::cout << "Error: ivalid args" << std::endl, 1);

    try 
    {
        RPN calc;
        int res = calc.eval(av[1]);
        std::cout << res << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
}