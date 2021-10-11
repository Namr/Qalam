#ifndef IR_H
#define IR_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

struct Variable
{
    uint32_t position;
    uint32_t width;
    Variable(uint32_t pposition, uint32_t wwidth);
};

class VariableList
{
private:
    uint32_t nextPosition = 0;
    std::map<std::string, Variable> vars;
public:
    void push_back(std::string name, uint32_t width);
    bool exists(std::string name);
    uint32_t size();
};

class Gate 
{
public:
    uint32_t numInputs;
    bool unboundedInputs;

    Gate(uint32_t nnumInputs);
    Gate(bool unbounded);
};

class GateList
{
public:
    std::map<std::string, Gate> gates;
    GateList();
    bool exists(std::string name);
};


struct BinaryExpression
{
    std::string variable;
    std::string gate;

    BinaryExpression(std::string vvar, std::string ggate);
};

#endif