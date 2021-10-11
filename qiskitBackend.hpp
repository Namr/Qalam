#ifndef QISKITBACKEND_H
#define QISKITBACKEND_H

#include <vector>

#include "ir.hpp"

class QiskitBackend
{
private:
    std::string pythonOutput;
public:
    QiskitBackend(VariableList& g_variables);
    void addBinaryExpression(BinaryExpression& expr, VariableList& g_variables, GateList& g_gates);
};

#endif