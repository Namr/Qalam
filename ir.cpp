#include "ir.hpp"

Variable::Variable(uint32_t pposition, uint32_t wwidth)
{
    width = wwidth;
    for (int i = pposition; i < pposition + width; i++)
    {
        positions.push_back(i);
    }
}

Variable::Variable()
{
    width = 0;
}

void Variable::concatenate(Variable &other)
{
    positions.insert(positions.end(), std::make_move_iterator(other.positions.begin()), std::make_move_iterator(other.positions.end()));
    width += other.width;
}

void VariableList::push_back(std::string name, uint32_t width)
{
    vars.insert(std::make_pair(name, Variable(nextPosition, width)));
    nextPosition += width;
}

bool VariableList::exists(std::string name)
{
    return vars.find(name) != vars.end();
}

uint32_t VariableList::size()
{
    return nextPosition;
}

Gate::Gate(uint32_t nnumInputs)
{
    numInputs = nnumInputs;
}

Gate::Gate(bool unbounded)
{
    unboundedInputs = unbounded;
}

GateList::GateList()
{
    gates.insert(std::make_pair("H", Gate(true)));
    gates.insert(std::make_pair("X", Gate(true)));
    gates.insert(std::make_pair("Y", Gate(true)));
    gates.insert(std::make_pair("Z", Gate(true)));
    gates.insert(std::make_pair("CX", Gate(true)));
}

bool GateList::exists(std::string name)
{
    return gates.find(name) != gates.end();
}

BinaryExpression::BinaryExpression(Variable *vvar, std::string ggate)
{
    variable = vvar;
    gate = ggate;
}