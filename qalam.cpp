#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "ir.hpp"
#include "qiskitBackend.hpp"

VariableList g_variables;
GateList g_gates;
QiskitBackend g_backend;

std::vector<CircuitExpression> statements;

void error(std::string error)
{
    std::cerr << "ERROR: " << error << std::endl;
    exit(1);
}

std::string match(Lexer &lex, Token t)
{
    LexItem n = lex.next();
    if (n.t != t)
    {
        error("Expected " + tokenToString(t) + " at position " + std::to_string(lex.index));
        return "";
    }

    return n.str;
}

void nextGate(Lexer &lex, std::string var)
{
    /*
    NEXTGATE -> "->" identifier NEXTGATE;
            | ;
    */
    if (lex.peek().t == ForwardArrow)
    {
        match(lex, ForwardArrow);
        std::string gateName = match(lex, Identifier);
        if (!g_gates.exists(gateName))
            error(gateName + " is a non-defined quantum gate");

        statements.push_back(CircuitExpression(var, gateName));
        nextGate(lex, var);
    }
    else if (lex.peek().t == SemiColon)
    {
        match(lex, SemiColon);
    }
    else
    {
        error("Expected ;");
    }
}

void specifier(Lexer &lex, std::string var)
{

    //SPECIFIER -> "->" identifier NEXTGATE
    //        | ,identifier SPECIFIER
    //        | [number] SPECIFIER

    if (lex.peek().t == ForwardArrow)
    {
        match(lex, ForwardArrow);

        //second must be a gate
        std::string gateName = match(lex, Identifier);
        if (!g_gates.exists(gateName))
            error(gateName + " is an undefined quantum gate");

        statements.push_back(CircuitExpression(var, gateName));

        nextGate(lex, var);
    }
    else if (lex.peek().t == Comma)
    {
        match(lex, Comma);
        std::string var2 = match(lex, Identifier);
        if (!g_variables.exists(var2))
            error(var2 + " is a variable that was never defined.");

        std::string combinedVarName = var + var2;
        if (!g_variables.exists(combinedVarName))
        {
            Variable combinedVar;
            combinedVar.concatenate(g_variables.vars[var]);
            combinedVar.concatenate(g_variables.vars[var2]);
            g_variables.push_back(combinedVarName, combinedVar);
        }

        specifier(lex, combinedVarName);
    }
    else if (lex.peek().t == OpenIndex)
    {
        match(lex, OpenIndex);
        std::string sIndex = match(lex, Number);
        int index = stoi(sIndex);

        std::string singleVarName = var + sIndex;
        if (!g_variables.exists(singleVarName))
        {
            Variable singleVar(g_variables.vars[var].positions[0] + index, 1);
            g_variables.push_back(singleVarName, singleVar);
        }
        match(lex, CloseIndex);
        specifier(lex, singleVarName);
    }
    else
    {
        error("invalid circuit statement");
    }
}

void statement(Lexer &lex)
{
    //STATEMENT -> identifier SPECIFIER

    //first identifier must be in symbol table
    std::string varName = match(lex, Identifier);
    if (!g_variables.exists(varName))
        error(varName + " is a variable that was never defined.");

    specifier(lex, varName);
}

void gateNextGate(Lexer &lex, std::string gateName, int var)
{
    /*
    NEXTGATE -> "->" identifier NEXTGATE;
            | ;
    */
    Gate *gate = &g_gates.gates[gateName];

    if (lex.peek().t == ForwardArrow)
    {
        //THIS PROBABLY DOESNT WORK!!!!!!!!!!!!!!!! NEEDS TESTING
        match(lex, ForwardArrow);
        std::string statementGate = match(lex, Identifier);
        if (!g_gates.exists(statementGate))
            error(statementGate + " is a non-defined quantum gate");
        
        GateExpression clone; 
        clone.vars = gate->statements[gate->statements.size() - 1].vars;
        clone.gate = statementGate;

        gate->statements.push_back(clone);
        gateNextGate(lex, gateName, var);
    }
    else if (lex.peek().t == SemiColon)
    {
        match(lex, SemiColon);
    }
    else
    {
        error("Expected ;");
    }
}

void gateSpecifier(Lexer &lex, std::string gateName, int var)
{
    Gate *gate = &g_gates.gates[gateName];

    //SPECIFIER -> "->" NEXTGATE
    //        | ,number SPECIFIER

    if (lex.peek().t == ForwardArrow)
    {
        match(lex, ForwardArrow);

        //second must be a gate
        std::string statementGate = match(lex, Identifier);
        if (!g_gates.exists(statementGate))
            error(statementGate + " is an undefined quantum gate");

        gate->statements[gate->statements.size() - 1].vars.push_back(var);
        gate->statements[gate->statements.size() - 1].gate = statementGate;

        gateNextGate(lex, gateName, var);
    }
    else if (lex.peek().t == Comma)
    {
        match(lex, Comma);
        std::string sInput2 = match(lex, Number);
        int var2 = stoi(sInput2);
        if (var2 > gate->numInputs || var2 < 0)
            error(std::to_string(var2) + " is an invalid input to gate " + gateName);

        gate->statements[gate->statements.size() - 1].vars.push_back(var);

        gateSpecifier(lex, gateName, var2);
    }
}

void gateStatement(Lexer &lex, std::string gateName)
{
    //STATEMENT -> number SPECIFIER
    //first identifier must be a valid number
    std::string sInput = match(lex, Number);
    int var = stoi(sInput);
    
    if (var > g_gates.gates[gateName].numInputs || var < 0)
        error(std::to_string(var) + " is an out of bounds input for the Gate " + gateName);

    //create a placeholder gate expression that will be populated by recursive calls
    g_gates.gates[gateName].statements.push_back(GateExpression());

    gateSpecifier(lex, gateName, var);
}

void gatebody(Lexer &lex, std::string gateName)
{
    //GATEBODY -> '{' {STATEMENT} '}'

    match(lex, OpenBody);
    while (lex.peek().t != CloseBody)
    {
        gateStatement(lex, gateName);
    }
    match(lex, CloseBody);
}

void definition(Lexer &lex, std::string name)
{
    /*
    DEFINITION -> ':' number INIT_QBIT
              | '<-' number "=" GATEBODY  
    */
    if (lex.peek().t == Colon)
    {
        match(lex, Colon);
        std::string swidth = match(lex, Number);
        g_variables.push_back(name, stoi(swidth));
    }
    else if (lex.peek().t == BackArrow)
    {
        match(lex, BackArrow);
        std::string inputSize = match(lex, Number);
        match(lex, Equals);
        g_gates.push_back(name, Gate(stoi(inputSize)));
        gatebody(lex, name);
    }
    else
    {
        error("bad identifier definition");
    }
}

void definitionStatements(Lexer &lex)
{
    //DEFSTATEMENT -> identifier DEFINITION;
    std::string name = match(lex, Identifier);

    //F is a reserved keyword for use inside of functions
    if (name[0] == 'F')
        error("non function variables are not allowed to start with F");

    definition(lex, name);
    match(lex, SemiColon);
}

int main(int argc, char** argv)
{
    char* filename = NULL;
    char* outputFile = "qalam_output.py";

    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-o") == 0)
        {
            if (argc > i)
                i++;
            else
                error("-o specified but no output file is given");
            
            outputFile = argv[i];
        }
        else
        {
            filename = argv[i];
        }
    }

    if(filename == NULL)
    {
        std::cout << "ERROR: invalid arguments" << std::endl;
        std::cout << "USAGE: qalam [OPTIONS] filename" << std::endl;
    }

    //open source file
    std::ifstream qalamInput;
    qalamInput.open(filename);
    if (qalamInput.is_open())
    {
        std::string line;
        std::getline(qalamInput, line);

        //each source file starts with a given: clause as seen in the grammer below
        //start -> "given:" {DEFSTATEMENT} "circuit:" {STATEMENT}
        if (line != "given:")
            error("no 'given:' section of program");

        //turn the entire 'given:' section into a single string to be tokenized
        std::string given_code = "";
        std::getline(qalamInput, line);
        while (line != "circuit:")
        {
            given_code += line;
            std::getline(qalamInput, line);
        }
        //tokenize the 'given:' section
        Lexer lex(given_code);

        //LL(1) parsing
        while (lex.peek().t != ERROR)
        {
            definitionStatements(lex);
        }

        //tokenize and parse the circuit section:
        given_code = "";
        while (std::getline(qalamInput, line))
        {
            given_code += line;
        }
        Lexer lex2(given_code);
        while (lex2.peek().t != ERROR)
        {
            statement(lex2);
        }

        //backend generation
        g_backend = QiskitBackend(g_variables);
        for (CircuitExpression e : statements)
        {
            g_backend.addBinaryExpression(e, g_variables, g_gates);
        }
        std::ofstream qalamOutput(outputFile);
        if(qalamOutput.is_open())
        {
            qalamOutput << g_backend.getOutput();
        }
        else
        {
            error("can't create output file");
        }
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}