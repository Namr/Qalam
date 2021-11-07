#ifndef IR_H
#define IR_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

struct Variable
{
    std::vector<int> positions;
    uint32_t width;
    Variable(uint32_t pposition, uint32_t wwidth);
    Variable();
    void concatenate(Variable& other);
};

class VariableList
{
private:
    uint32_t nextPosition = 0;
public:
    std::map<std::string, Variable> vars;
    void push_back(std::string name, uint32_t width);
    void push_back(std::string name, Variable var);
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
    std::string var;
    std::string gate;

    BinaryExpression(std::string vvar, std::string ggate);
};

#endif