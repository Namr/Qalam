#include <iostream>
#include <fstream>
#include <string>

#include "lexer.hpp"

class Statement
{
};

void error(std::string error)
{
    std::cerr << "ERROR: " << error << std::endl;
    exit(1);
}

void match(Lexer &lex, Token t)
{
    if (lex.next() != t)
    {
        error("Expected " + tokenToString(t) + " at position " + std::to_string(lex.index));
    }
}

void nextgate(Lexer &lex)
{
    /*
    NEXTGATE -> "->" identifier NEXTGATE;
            | ;
    */
    if (lex.peek() == ForwardArrow)
    {
        match(lex, ForwardArrow);
        match(lex, Identifier);
        nextgate(lex);
    }
    else if (lex.peek() == SemiColon)
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
    match(lex, Identifier);
    match(lex, ForwardArrow);
    match(lex, Identifier);
    nextgate(lex);
}

void gatebody(Lexer &lex)
{
    //GATEBODY -> '{' {STATEMENT} '}'

    match(lex, OpenBody);
    while (lex.peek() != CloseBody)
    {
        statement(lex);
    }
    match(lex, CloseBody);
}

void definition(Lexer &lex)
{
    /*
    DEFINITION -> ':' number INIT_QBIT
              | '<-' number "=" GATEBODY  
    */
    if (lex.peek() == Colon)
    {
        match(lex, Colon);
        match(lex, Number);
    }
    else if (lex.peek() == BackArrow)
    {
        match(lex, BackArrow);
        match(lex, Number);
        match(lex, Equals);
        gatebody(lex);
    }
    else
    {
        error("Error! bad identifier definition");
    }
}

void definition_statements(Lexer &lex)
{
    //DEFSTATEMENT -> identifier DEFINITION;
    match(lex, Identifier);
    definition(lex);
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
        while (lex.peek() != ERROR)
        {
            definition_statements(lex);
        }
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}