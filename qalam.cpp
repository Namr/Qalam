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

void match(Lexer& lex, Token t)
{
    if(lex.next() != t)
    {
        error("Error! Expected " + tokenToString(t));
    }
}
int definition_statements(Lexer& lex)
{
    //DEFSTATEMENT -> identifier DEFINITION

    return 0;
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
        if(line != "given:")
            error("no 'given:' section of program");

        //turn the entire 'given:' section into a single string to be tokenized
        std::string given_code = "";
        std::getline(qalamInput, line);
        while(line != "circut:")
        {
            given_code += line;
            std::getline(qalamInput, line);
        }
        //tokenize the 'given:' section
        Lexer lex(given_code);

        //LL(1) parsing
        definition_statements(lex);
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}