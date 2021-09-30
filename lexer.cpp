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
        return "ERROR";
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
semicolRStr(";", std::regex_constants::ECMAScript),
fullRStr("(;)|(=)|(:)|(->)|(<-)|(\\{)|(\\})|([a-zA-Z]([a-zA-Z]|[0-9])*)|([0-9]+)", std::regex_constants::ECMAScript)
{

    index = 0;
    std::string::const_iterator searchStart( input.cbegin() );
    std::smatch sm;

    //keep matching tokens until none are left
    while(std::regex_search(searchStart, input.cend(), sm, fullRStr))
    {
        //add the right token into the list of tokens
        if(std::regex_match(sm[0].str(), idRStr))
            tokens.push_back(Identifier);
        else if(std::regex_match(sm[0].str(), numRStr))
            tokens.push_back(Number);
        else if(std::regex_match(sm[0].str(), eqRStr))
            tokens.push_back(Equals);
        else if(std::regex_match(sm[0].str(), colRStr))
            tokens.push_back(Colon);
        else if(std::regex_match(sm[0].str(), faRStr))
            tokens.push_back(ForwardArrow);
        else if(std::regex_match(sm[0].str(), baRStr))
            tokens.push_back(BackArrow);
        else if(std::regex_match(sm[0].str(), obRStr))
            tokens.push_back(OpenBody);
        else if(std::regex_match(sm[0].str(), cbRStr))
            tokens.push_back(CloseBody);
        else if(std::regex_match(sm[0].str(), semicolRStr))
            tokens.push_back(SemiColon);

        //move to the character after the end of the match in the string
        searchStart = sm.suffix().first;
    }

    for(Token t : tokens)
    {
        std::cout << tokenToString(t) << " ";
    }
    std::cout << std::endl;
}

//see what the next token is
Token Lexer::peek()
{
    if(index < tokens.size() && index >= 0)
        return tokens[index];
    else
        return ERROR;
}

//consume the next token
Token Lexer::next()
{
    Token p = peek();
    index++;
    return p;
}
