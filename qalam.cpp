#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "ir.hpp"
#include "qiskitBackend.hpp"
#include "qalamFrontend.hpp"
#include "quirkFrontend.hpp"

int main(int argc, char **argv)
{
    /////////////////////////////////////////////////////////////////////////////////////////////
    //get command line arguments and figure out which files we are reading from and writing to//
    ////////////////////////////////////////////////////////////////////////////////////////////
    char *filename = NULL;
    char *outputFile = "qalam_output.py";
    bool useQuirkFrontend = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (argc > i)
                i++;
            else
                error("-o specified but no output file is given");

            outputFile = argv[i];
        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            useQuirkFrontend = true;
        }
        else
        {
            filename = argv[i];
        }
    }

    if (filename == NULL)
    {
        std::cout << "ERROR: invalid arguments" << std::endl;
        std::cout << "USAGE: qalam [OPTIONS] filename" << std::endl;
    }

    /////////////////////////////////////////////////////////////
    // Based on frontend choice, create an IR through a parser//
    ///////////////////////////////////////////////////////////
    if (useQuirkFrontend)
    {
        Quirk::parseJSON(filename);
    }
    else
    {
        ///////////////////////////////////////////
        // Do LL(1) parsing on an input QLM File//
        //////////////////////////////////////////
        
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
                Qalam::definitionStatements(lex);
            }

            //tokenize and LL(1) parse the circuit section:
            given_code = "";
            while (std::getline(qalamInput, line))
            {
                given_code += line;
            }
            Lexer lex2(given_code);
            while (lex2.peek().t != ERROR)
            {
                Qalam::statement(lex2);
            }
        }
        else
        {
            error("can't open input file");
        }

        qalamInput.close();
    }

    ////////////////////////////////////////////////
    //Use generated IR to produce a backend output//
    ////////////////////////////////////////////////
    QiskitBackend backend = QiskitBackend(g_variables);
    for (CircuitExpression e : g_statements)
    {
        backend.addBinaryExpression(e, g_variables, g_gates);
    }
    std::ofstream qalamOutput(outputFile);
    if (qalamOutput.is_open())
    {
        qalamOutput << backend.getOutput();
    }
    else
    {
        error("can't create output file");
    }
}