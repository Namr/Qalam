#include "lexer.hpp"

std::string tokenToString(Token t)
{
    switch (t)
    {
    case Identifier:
        return "Identifier";
    case Number:
        return "Number";
    case Equals:
        return "=";
    case Colon:
        return ":";
    case ForwardArrow:
        return "->";
    case BackArrow:
        return "<-";
    case OpenBody:
        return "{";
    case CloseBody:
        return "}";
    case SemiColon:
        return ";";
    case ERROR:
    default:
        return "ERROR";
    }
}

Lexer::Lexer(std::string input) :
idRStr("[a-zA-Z]([a-zA-Z]|[0-9])*", std::regex_constants::ECMAScript),
numRStr("[0-9]*", std::regex_constants::ECMAScript),
eqRStr("=", std::regex_constants::ECMAScript),
colRStr(":",std::regex_constants::ECMAScript),
faRStr("->", std::regex_constants::ECMAScript),
baRStr("<-", std::regex_constants::ECMAScript),
obRStr("\\{", std::regex_constants::ECMAScript),
cbRStr("\\}", std::regex_constants::ECMAScript),
commaRStr(",", std::regex_constants::ECMAScript),
oiRStr("\\[", std::regex_constants::ECMAScript),
ciRStr("\\]", std::regex_constants::ECMAScript),
semicolRStr(";", std::regex_constants::ECMAScript),
fullRStr("(;)|(\\[)|(\\])|(,)|(=)|(:)|(->)|(<-)|(\\{)|(\\})|([a-zA-Z]([a-zA-Z]|[0-9])*)|([0-9]+)", std::regex_constants::ECMAScript)
{

    index = 0;
    std::string::const_iterator searchStart( input.cbegin() );
    std::smatch sm;

    //keep matching tokens until none are left
    while(std::regex_search(searchStart, input.cend(), sm, fullRStr))
    {
        //add the right token into the list of tokens
        if(std::regex_match(sm[0].str(), idRStr))
            tokens.push_back(LexItem(Identifier, sm[0].str()));
        else if(std::regex_match(sm[0].str(), numRStr))
            tokens.push_back(LexItem(Number, sm[0].str()));
        else if(std::regex_match(sm[0].str(), eqRStr))
            tokens.push_back(LexItem(Equals, sm[0].str()));
        else if(std::regex_match(sm[0].str(), colRStr))
            tokens.push_back(LexItem(Colon, sm[0].str()));
        else if(std::regex_match(sm[0].str(), faRStr))
            tokens.push_back(LexItem(ForwardArrow, sm[0].str()));
        else if(std::regex_match(sm[0].str(), baRStr))
            tokens.push_back(LexItem(BackArrow, sm[0].str()));
        else if(std::regex_match(sm[0].str(), obRStr))
            tokens.push_back(LexItem(OpenBody, sm[0].str()));
        else if(std::regex_match(sm[0].str(), cbRStr))
            tokens.push_back(LexItem(CloseBody, sm[0].str()));
        else if(std::regex_match(sm[0].str(), semicolRStr))
            tokens.push_back(LexItem(SemiColon, sm[0].str()));
        else if(std::regex_match(sm[0].str(), commaRStr))
            tokens.push_back(LexItem(Comma, sm[0].str()));
        else if(std::regex_match(sm[0].str(), oiRStr))
            tokens.push_back(LexItem(OpenIndex, sm[0].str()));
        else if(std::regex_match(sm[0].str(), ciRStr))
            tokens.push_back(LexItem(CloseIndex, sm[0].str()));
        //move to the character after the end of the match in the string
        searchStart = sm.suffix().first;
    }
}

//see what the next token is
LexItem Lexer::peek()
{
    if(index < tokens.size() && index >= 0)
        return tokens[index];
    else
        return LexItem(ERROR, "");
}

//consume the next token
LexItem Lexer::next()
{
    LexItem p = peek();
    index++;
    return p;
}
