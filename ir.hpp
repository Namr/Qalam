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

struct CircuitExpression
{
    std::string var;
    std::string gate;

    CircuitExpression(std::string vvar, std::string ggate);
};

struct GateExpression
{
    std::vector<int> vars;
    std::string gate;

    GateExpression();
    GateExpression(int vvar, std::string ggate);
    void addVar(int var);
    bool isSingleInput();
};

class Gate 
{
public:
    uint32_t numInputs = 0;
    std::vector<GateExpression> statements;
    Gate();
    Gate(uint32_t nnumInputs);
};

class GateList
{
public:
    GateList();
    std::map<std::string, Gate> gates;
    bool exists(std::string name);
    void push_back(std::string name, Gate gate);
};

//declare global IR variables
extern VariableList g_variables;
extern GateList g_gates;
extern std::vector<CircuitExpression> g_statements;

#endif