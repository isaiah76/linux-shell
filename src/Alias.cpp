#include "Alias.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

std::string applyAlias(Shell* shell, const std::string& cmd) {
    std::vector<std::string> tokens = shell->tokenize(cmd);
    if(tokens.empty())
        return cmd;
    auto& aliases = shell->getAliases();
    auto it = aliases.find(tokens[0]);
    if(it != aliases.end()){
        std::string result = it->second;
        for(size_t i = 1; i < tokens.size(); i++){
            result += " " + tokens[i];
        }
        return result;
    }
    return cmd;
}

void loadAliases(Shell* shell) {
    const char* home = getenv("HOME");
    std::string filename = std::string(home) + "/.linux_shell_aliases";
    std::ifstream file(filename);
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            size_t pos = line.find('=');
            if(pos != std::string::npos){
                std::string alias = line.substr(0, pos);
                std::string command = line.substr(pos + 1);
                shell->getAliases()[alias] = command;
            }
        }
        file.close();
    }
}

void saveAliases(Shell* shell) {
    const char* home = getenv("HOME");
    std::string filename = std::string(home) + "/.linux_shell_aliases";
    std::ofstream file(filename);
    if(file.is_open()){
        for(const auto& pair : shell->getAliases()){
            file << pair.first << "=" << pair.second << std::endl;
        }
        file.close();
    }
}

