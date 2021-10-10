#include "ast.hpp"

Expression::Expression(std::string op)
{
    this->value = op;
}

std::ostream& operator << (std::ostream& outs, const Expression& e)
{
    return outs << e.value;
}

BinaryExpression::BinaryExpression(std::string op, Expression* first, Expression* second) : Expression(op)
{
    this->first = first;
    this->second = second;
}

std::ostream& operator << (std::ostream& outs, const BinaryExpression& be)
{
    return outs << be.value << " " << be.first << " " << be.second;
}

