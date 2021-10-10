#ifndef AST_H
#define AST_H
#include <string>
#include <iostream>

struct Variable
{
    std::string name;
    uint32_t width;
    Variable(std::string nname, uint32_t wwidth)
    {
        name = nname;
        width = wwidth;
    }
};

class Expression
{ 
public:
    std::string value;
    Expression(std::string value);
};

class BinaryExpression: public Expression
{
public:
    Expression* first;
    Expression* second;

    BinaryExpression(std::string op, Expression* second, Expression* third);
};

#endif