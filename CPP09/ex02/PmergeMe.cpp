#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other)
{
    *this = other;
}
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        vect = other.vect;
        deq = other.deq;
    }
    return *this;
}

bool isValid(const std::string &str)
{
    if (str.empty())
        return 0;
    for (size_t i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            return 0;
    long num = atol(str.c_str());
    if (num < 0 || num > INT_MAX)
        return 0;
    return 1;
}

void PmergeMe::parse(int ac, char **av)
{
    for (int i = 1; i < ac; i++)
    {
        std::string str = av[i];
        if (!isValid(str))
            throw std::runtime_error("invalid input");
        
        int num = atoi(str.c_str());
        if (num <= 0)
            throw std::runtime_error("only positive nums allowed");
        vect.push_back(num);
        deq.push_back(num);
    }
}

void display(const std::vector<int> &vect)
{
    for (size_t i = 0; i < vect.size(); i++)
    {
        std::cout << vect[i];
        if (i < vect.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
}

void fjDeque(std::deque<int> &arr)
{
    if (arr.size() <= 1) return;
    if (arr.size() <= 16)
    {
        for (size_t i = 1; i < arr.size(); i++)
        {
            int tmp = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > tmp)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = tmp;
        }
        return;
    }
    std::deque<std::pair<int, int> > pair;
    for (size_t i = 0; i < arr.size() - 1; i += 2)
    {
        int a = arr[i];
        int b = arr[i + 1];
        if (a > b)
            std::swap(a, b);
        pair.push_back(std::make_pair(a, b));
    }
    bool odd = (arr.size() % 2 == 1);
    int oddNum;
    if (odd) oddNum = arr.back();
    else     oddNum = 0;

    std::deque<int> biggerNums;
    for (size_t i = 0; i < pair.size(); i++)
        biggerNums.push_back(pair[i].second);
    fjDeque(biggerNums);

    std::deque<int> sorted;
    for (size_t i = 0; i < biggerNums.size(); i++)
        sorted.push_back(biggerNums[i]);

    for (size_t i = 0; i < pair.size(); i++)
    {
        int smaller = pair[i].first;
        std::deque<int>::iterator pos = std::lower_bound(sorted.begin(), sorted.end(), smaller);
        sorted.insert(pos, smaller);
    }
    if (odd)
    {
        std::deque<int>::iterator pos = std::lower_bound(sorted.begin(), sorted.end(), oddNum);
        sorted.insert(pos, oddNum);
    }
    arr = sorted;
}

void fjVector(std::vector<int> &arr)
{
    if (arr.size() <= 1) return;
    if (arr.size() <= 16)
    {
        for (size_t i = 1; i < arr.size(); i++)
        {
            int tmp = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > tmp)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = tmp;
        }
        return;
    }

    std::vector<std::pair<int, int> > pair;
    for (size_t i = 0; i < arr.size() - 1; i += 2)
    {
        int a = arr[i];
        int b = arr[i + 1];
        if (a > b)
            std::swap(a, b);
        pair.push_back(std::make_pair(a, b));
    }

    bool odd = (arr.size() % 2 == 1);
    int oddNum;
    if (odd) oddNum = arr.back();
    else     oddNum = 0;

    std::vector<int> biggerNums;
    for (size_t i = 0; i < pair.size(); i++)
        biggerNums.push_back(pair[i].second);
    fjVector(biggerNums);

    std::vector<int> sorted;
    for (size_t i = 0; i < biggerNums.size(); i++)
        sorted.push_back(biggerNums[i]);

    for (size_t i = 0; i < pair.size(); i++)
    {
        int smaller = pair[i].first;
        std::vector<int>::iterator pos = std::lower_bound(sorted.begin(), sorted.end(), smaller);
        sorted.insert(pos, smaller);
    }

    if (odd)
    {
        std::vector<int>::iterator pos = std::lower_bound(sorted.begin(), sorted.end(), oddNum);
        sorted.insert(pos, oddNum);
    }
    arr = sorted;
}


void PmergeMe::sort()
{
    std::cout << "Before: ";
    display(vect);

    std::vector<int> vectcpy = vect;
    clock_t vectStart = clock();
    fjVector(vectcpy);
    clock_t vectEnd = clock();
    double timeVect = double(vectEnd - vectStart) / CLOCKS_PER_SEC * 1000000;

    std::deque<int> deqcpy = deq;
    clock_t deqStart = clock();
    fjDeque(deqcpy);
    clock_t deqEnd = clock();
    double timeDeq = double(deqEnd - deqStart) / CLOCKS_PER_SEC * 1000000;

    std::cout << "After: ";
    display(vectcpy);
    std::cout << "Time to process a range of " << vect.size();
    std::cout << " elements with std::vector: " << timeVect << " us" << std::endl;
    std::cout << "Time to process a range of " << deq.size();
    std::cout << " elements with std::deque: " << timeDeq << " us" << std::endl;
}
