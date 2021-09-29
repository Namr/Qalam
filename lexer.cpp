#include "lexer.hpp"

Lexer::Lexer(std::string input)
{
    index = 0;
    const std::regex r(fullRStr, std::regex_constants::basic);

    std::smatch sm;

    if (regex_search(input, sm, r)) {
        for (int i=1; i<sm.size(); i++) {
            std::cout << sm[i] << std::endl;
        }
    }
}


Token Lexer::peek()
{
    if(index < tokens.size() && index >= 0)
        return tokens[index+1];
    else
        return Token::ERROR;
}

Token Lexer::next()
{
    index++;
    return peek();
}
