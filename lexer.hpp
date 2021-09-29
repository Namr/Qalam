#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>

enum class Token
{
    Identifier,
    Number,
    Equals,
    Colon,
    ForwardArrow,
    BackArrow,
    OpenBody,
    CloseBody,
    ERROR,
};

class Lexer
{
private:
    std::vector<Token> tokens;
    int index;

    const std::string idRStr = "[a-zA-Z]([a-zA-Z]|[0-9])*";
    const std::string numRStr = "[0-9]*";
    const std::string eqRStr = "=";
    const std::string colRStr = ":";
    const std::string faRStr = "->";
    const std::string baRStr = "<-";
    const std::string obRStr = "{";
    const std::string cbRStr = "}";

    const std::string fullRStr = "=|:|->|<-|{|}|[a-zA-Z]([a-zA-Z]|[0-9])*|[0-9]*";
    
public:
    Lexer(std::string input);
    Token peek();
    Token next();
};

#endif