#ifndef ALIAS_HPP
#define ALIAS_HPP

#include "Shell.hpp"
#include <string>
#include <vector>

std::string applyAlias(Shell* shell, const std::string& cmd);

// Load and save aliases from/to a file (~/.linux_shell_aliases)
void loadAliases(Shell* shell);
void saveAliases(Shell* shell);

#endif // ALIAS_HPP

