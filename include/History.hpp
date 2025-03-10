#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "Shell.hpp"

// Load and save command history to/from a file (~/.linux_shell_history)
void loadHistory(Shell* shell);
void saveHistory(Shell* shell);

#endif // HISTORY_HPP

