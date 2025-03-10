#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <string>
#include "Shell.hpp"

// execute external commands.
void execExternal(Shell* shell, const std::vector<std::string>& tokens);

#endif // EXECUTOR_HPP

