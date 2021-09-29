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

int definition_statements(Lexer& lex)
{
    //DEFSTATEMENT -> identifier DEFINITION

}

int main()
{
    std::ifstream qalamInput;
    qalamInput.open("test.qlm");
    if (qalamInput.is_open())
    {
        std::string line;
        std::getline(qalamInput, line);

        //start -> "given:" {DEFSTATEMENT} "circut:" {STATEMENT}
        if(line != "given:")
            error("no 'given:' section of program");

        while(line != "circut:")
        {
            std::getline(qalamInput, line);
            Lexer lex(line);
            //definition_statements(lex);
        }
    }
    else
    {
        error("can't open input file");
    }

    qalamInput.close();
}