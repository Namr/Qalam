#include "qiskitBackend.hpp"

QiskitBackend::QiskitBackend(VariableList& g_variables)
{
    pythonOutput += "from qiskit import QuantumCircuit, assemble, Aer\n";
    pythonOutput += "from math import pi, sqrt\n";
    pythonOutput += "from qiskit.visualization import plot_bloch_multivector, plot_histogram\n";
    pythonOutput += "sim = Aer.get_backend('aer_simulator')\n";

    pythonOutput += "qc = QuantumCircuit(" + std::to_string(g_variables.size()) + ")";
}

void QiskitBackend::addBinaryExpression(BinaryExpression& expr, VariableList& g_variables, GateList& g_gates)
{

}