#pragma once

#include <stack>
#include <iostream>

class RPN
{
    private:
        std::stack<int> Stack;

        bool isOperator(const std::string &token) const;
        bool isNumber(const std::string &token) const;
        int operate(int a, int b, char op);
    public:
        RPN();
        ~RPN();
        RPN(const RPN &other);
        RPN &operator=(const RPN &other);
        
        int eval(const std::string &input);
};