#include "RPN.hpp"
#include <sstream>
#include <cstdlib>

RPN::RPN() {}
RPN::~RPN() {}
RPN::RPN(const RPN &other) 
{ 
    *this = other;
}
RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        Stack = other.Stack;
    return *this;
}
bool RPN::isOperator(const std::string &token) const
{
    if (token == "+" || token == "-" || token == "*" || token == "/")
        return 1;
    return 0;
}
bool RPN::isNumber(const std::string &token) const
{
    if (token.empty())
        return 0;
    for (size_t i = 0; i < token.size(); i++)
        if (!isdigit(token[i]))
            return 0;
    if (atoi(token.c_str()) < 0 || atoi(token.c_str()) > 9)
        return 0;
    return 1;
}

int RPN::operate(int a, int b, char op)
{
    switch (op) 
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0)
                throw std::runtime_error("Error: division by zero");
            return a / b;
        default: 
            throw std::runtime_error("Error: invalid operator");
    }
}

int RPN::eval(const std::string &input)
{
    while (!Stack.empty())
        Stack.pop();
    std::istringstream isString(input);
    std::string token;
    while (isString >> token)
    {
        if (isNumber(token))
            Stack.push(atoi(token.c_str()));

        else if (isOperator(token))
        {
            if (Stack.size() < 2)
                throw std::runtime_error("Error: not enough numbers");
            
            int a = Stack.top();
                Stack.pop();
            int b = Stack.top();
                Stack.pop();
            int res = operate(b, a, token[0]);
            Stack.push(res);
        }
        else
            throw std::runtime_error("Error: invalid token");
    }
    if (Stack.size() != 1)
            throw std::runtime_error("Error: invalid expression");
    return Stack.top();
}
