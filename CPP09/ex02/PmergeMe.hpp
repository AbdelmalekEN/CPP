#pragma once

#include <vector>
#include <deque>
#include <iostream>

class PmergeMe
{
    private:
        std::vector<int> vect;
        std::deque<int> deq;
    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void parse(int ac, char **av);
        void sort();
};