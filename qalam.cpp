#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "ir.hpp"
#include "qiskitBackend.hpp"

VariableList g_variables;
std::vector<Variable> g_combinedVars;
GateList g_gates;
QiskitBackend g_backend;

std::vector<BinaryExpression> statements;

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

void nextGate(Lexer &lex, Variable *var)
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

        statements.push_back(BinaryExpression(var, gateName));
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

void specifier(Lexer &lex, Variable *var)
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

        statements.push_back(BinaryExpression(var, gateName));

        nextGate(lex, var);
    }
    else if (lex.peek().t == Comma)
    {
        match(lex, Comma);
        std::string var2Name = match(lex, Identifier);
        if (!g_variables.exists(var2Name))
            error(var2Name + " is a variable that was never defined.");

        Variable combinedVar;
        combinedVar.concatenate(*var);
        combinedVar.concatenate(g_variables.vars[var2Name]);
        g_combinedVars.push_back(combinedVar);
        specifier(lex, &g_combinedVars[g_combinedVars.size() - 1]);
    }
    else if (lex.peek().t == OpenIndex)
    {
        match(lex, OpenIndex);
        std::string sIndex = match(lex, Number);
        int index = stoi(sIndex);

        Variable singleVar(var->positions[0] + index, 1);
        g_combinedVars.push_back(singleVar);
        match(lex, CloseIndex);
        specifier(lex, &g_combinedVars[g_combinedVars.size() - 1]);
    }
    else
    {
        error("invalid circut statement");
    }
}

void statement(Lexer &lex)
{
    //STATEMENT -> identifier SPECIFIER

    //first identifier must be in symbol table
    std::string varName = match(lex, Identifier);
    if (!g_variables.exists(varName))
        error(varName + " is a variable that was never defined.");

    specifier(lex, &g_variables.vars[varName]);
}

void gateNextGate(Lexer &lex)
{
    /*
    NEXTGATE -> "->" identifier NEXTGATE;
            | ;
    */
    if (lex.peek().t == ForwardArrow)
    {
        match(lex, ForwardArrow);
        match(lex, Identifier);
        gateNextGate(lex);
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

void gateStatement(Lexer &lex)
{
    //STATEMENT -> identifier "->" identifier NEXTGATE
    match(lex, Identifier);
    match(lex, ForwardArrow);
    match(lex, Identifier);
    gateNextGate(lex);
}

void gatebody(Lexer &lex)
{
    //GATEBODY -> '{' {STATEMENT} '}'

    match(lex, OpenBody);
    while (lex.peek().t != CloseBody)
    {
        gateStatement(lex);
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
        match(lex, Number);
        match(lex, Equals);
        gatebody(lex);
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

int main()
{
    //open source file
    std::ifstream qalamInput;
    qalamInput.open("test.qlm");
    if (qalamInput.is_open())
    {
        std::string line;
        std::getline(qalamInput, line);

        //each source file starts with a given: clause as seen in the grammer below
        //start -> "given:" {DEFSTATEMENT} "circut:" {STATEMENT}
        if (line != "given:")
            error("no 'given:' section of program");

        //turn the entire 'given:' section into a single string to be tokenized
        std::string given_code = "";
        std::getline(qalamInput, line);
        while (line != "circut:")
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

        //tokenize and parse the circut section:
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
        for (BinaryExpression e : statements)
        {
            g_backend.addBinaryExpression(e, g_variables, g_gates);
        }
        g_backend.printOutput();
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}