#ifndef QUIRKFRONTEND_H
#define QUIRKFRONTEND_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "lexer.hpp"
#include "ir.hpp"

namespace Quirk
{
    void parseJSON(std::string filename);
}
#endif