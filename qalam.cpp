#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "ast.hpp"

std::vector<Variable> g_variables;

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

void nextGate(Lexer &lex)
{
    /*
    NEXTGATE -> "->" identifier NEXTGATE;
            | ;
    */
    if (lex.peek().t == ForwardArrow)
    {
        match(lex, ForwardArrow);
        match(lex, Identifier);
        nextGate(lex);
    }
    else if (lex.peek().t == SemiColon)
    {
        match(lex, SemiColon);
    }
    else
    {
        error("Error! Expected ;");
    }
}

void statement(Lexer &lex)
{
    //STATEMENT -> identifier "->" identifier NEXTGATE

    //first identifier must be in symbol table
    std::string varName = match(lex, Identifier);
    match(lex, ForwardArrow);

    //second must be a gate
    std::string gateName = match(lex, Identifier);
    nextGate(lex);
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
        error("Error! Expected ;");
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
        g_variables.push_back(Variable(name, stoi(swidth)));
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
    if(name[0] == 'F')
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
        
        given_code = "";
        //tokenize and parse the circut section:
        while(std::getline(qalamInput, line))
        {
            given_code += line;
        }

        Lexer lex2(given_code);

        while(lex.peek().t != ERROR)
        {
            statement(lex);
        }
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}