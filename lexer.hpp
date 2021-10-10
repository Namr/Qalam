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

struct LexItem
{
    Token t;
    std::string str;
    LexItem(Token tt, std::string sstr)
    {
        t = tt;
        str = sstr;
    }
};

std::string tokenToString(Token t);

class Lexer
{
private:
    std::vector<LexItem> tokens;

    const std::regex idRStr, numRStr,
        eqRStr, colRStr, faRStr, semicolRStr,
        baRStr, obRStr, cbRStr, fullRStr;

public:
    Lexer(std::string input);
    LexItem peek();
    LexItem next();

    int index;
};

#endif