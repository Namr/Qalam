#include "qiskitBackend.hpp"

QiskitBackend::QiskitBackend(VariableList &g_variables)
{
    pythonOutput += "from qiskit import QuantumCircuit, assemble, Aer\n";
    pythonOutput += "from math import pi, sqrt\n";
    pythonOutput += "from qiskit.visualization import plot_bloch_multivector, plot_histogram\n";
    pythonOutput += "sim = Aer.get_backend('aer_simulator')\n";
    pythonOutput += "from qiskit.circuit.library.standard_gates import *\n";

    pythonOutput += "qc = QuantumCircuit(" + std::to_string(g_variables.size()) + ")\n";
}

QiskitBackend::QiskitBackend() {}

void QiskitBackend::addBinaryExpression(CircuitExpression &expr, VariableList &g_variables, GateList &g_gates)
{
    // Handle the default gates that come with Qiskit
    if (expr.gate == "H" || expr.gate == "X" || expr.gate == "Y" || expr.gate == "Z" || expr.gate == "CX")
    {
        CreatePythonStatement(g_variables.vars[expr.var].positions, expr.gate);
    }
    else
    {
        //go through each Gatestatment and make a corresponding circuit statement for it and then have the backend handle that circuitstatment
        for (GateExpression e : g_gates.gates[expr.gate].statements)
        {
            std::vector<int> realPositions;
            for(int p : e.vars)
            {
                realPositions.push_back(g_variables.vars[expr.var].positions[p]);
            }
            CreatePythonStatement(realPositions, e.gate);
        }
    }
}

void QiskitBackend::CreatePythonStatement(std::vector<int> positions, std::string gate)
{
    if (gate == "H" || gate == "X" || gate == "Y" || gate == "Z")
    {
        std::string c;
        c.push_back(tolower(gate[0]));
        for (int p : positions)
        {
            pythonOutput += "qc." + c;
            pythonOutput += "(" + std::to_string(p) + ")\n";
        }
    }
    else if (gate == "CX")
    {
        pythonOutput += "qc.append(XGate().control(" + std::to_string(positions.size() - 1) + "),";
        pythonOutput += "[";

        for (int i = 0; i < positions.size() - 1; i++)
        {
            pythonOutput += std::to_string(positions[i]);
            pythonOutput += ",";
        }
        pythonOutput += std::to_string(positions[positions.size() - 1]);
        pythonOutput += "])\n";
    }
}

std::string QiskitBackend::getOutput()
{
    return pythonOutput;
}