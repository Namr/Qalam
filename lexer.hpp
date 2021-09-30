#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>

enum Token
{
    Identifier,
    Number,
    Equals,
    Colon,
    ForwardArrow,
    BackArrow,
    OpenBody,
    CloseBody,
    SemiColon,
    ERROR,
};

std::string tokenToString(Token t);

class Lexer
{
private:
    std::vector<Token> tokens;
    int index;

    const std::regex idRStr, numRStr,
        eqRStr, colRStr, faRStr, semicolRStr,
        baRStr, obRStr, cbRStr, fullRStr;

public:
    Lexer(std::string input);
    Token peek();
    Token next();
};

#endif