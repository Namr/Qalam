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

void QiskitBackend::addBinaryExpression(BinaryExpression &expr, VariableList &g_variables, GateList &g_gates)
{
    // Handle the default gates that come with Qiskit
    if (expr.gate == "H" || expr.gate == "X" || expr.gate == "Y" || expr.gate == "Z")
    {
        std::string c;
        c.push_back(tolower(expr.gate[0]));

        for (int p : expr.variable->positions)
        {
            pythonOutput += "qc." + c;
            pythonOutput += "(" + std::to_string(p) + ")\n";
        }
    }
    if (expr.gate == "CX")
    {
        pythonOutput += "qc.append(XGate().control(" + std::to_string(expr.variable->width - 1) + "),";
        pythonOutput += "[";

        for (int i = 0; i < expr.variable->positions.size() - 1; i++)
        {
            pythonOutput += std::to_string(expr.variable->positions[i]);
            pythonOutput += ",";
        }
        pythonOutput += std::to_string(expr.variable->positions[expr.variable->positions.size()-1]);
        pythonOutput += "])\n";
    }
}

void QiskitBackend::printOutput()
{
    std::cout << pythonOutput;
}