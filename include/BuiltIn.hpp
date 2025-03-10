#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <vector>
#include <string>
#include "Shell.hpp"

// execute builtin commands 
bool execBuiltIn(Shell* shell, const std::vector<std::string>& tokens);

#endif // BUILTIN_HPP

