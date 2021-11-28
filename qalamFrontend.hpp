#ifndef QALAMFRONTEND_H
#define QALAMFRONTEND_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "ir.hpp"
#include "qiskitBackend.hpp"

void error(std::string error);

namespace Qalam
{
    std::string match(Lexer &lex, Token t);
    void nextGate(Lexer &lex, std::string var);
    void specifier(Lexer &lex, std::string var);
    void statement(Lexer &lex);
    void gateNextGate(Lexer &lex, std::string gateName, int var);
    void gateSpecifier(Lexer &lex, std::string gateName, int var);
    void gateStatement(Lexer &lex, std::string gateName);
    void gatebody(Lexer &lex, std::string gateName);
    void definition(Lexer &lex, std::string name);
    void definitionStatements(Lexer &lex);
}

#endif