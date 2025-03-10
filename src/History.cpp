#include "History.hpp"
#include <fstream>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>

void loadHistory(Shell* shell) {
    const char* home = getenv("HOME");
    std::string filename = std::string(home) + "/.linux_shell_history";
    std::ifstream file(filename);
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            shell->getHistory().push_back(line);
            add_history(line.c_str());
        }
        file.close();
    }
}

void saveHistory(Shell* shell) {
    const char* home = getenv("HOME");
    std::string filename = std::string(home) + "/.linux_shell_history";
    std::ofstream file(filename);
    if(file.is_open()){
        for(const auto& cmd : shell->getHistory()){
            file << cmd << std::endl;
        }
        file.close();
    }
}

