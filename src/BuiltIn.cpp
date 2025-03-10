#include "BuiltIn.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <map>

// built-in commands.
bool execBuiltIn(Shell* shell, const std::vector<std::string>& tokens) {
    if(tokens.empty())
        return true;

    if(tokens[0] == "exit"){
        exit(0);
        return true;
    } else if(tokens[0] == "echo"){
        for(size_t i = 1; i < tokens.size(); ++i){
            if(i > 1)
                std::cout << " ";
            std::cout << tokens[i];
        }
        std::cout << std::endl;
        return true;
    } else if(tokens[0] == "cd"){
        if(tokens.size() < 2){
            const char* home = getenv("HOME");
            if(home && chdir(home) == 0){
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) != nullptr)
                    shell->updateCurrentDir(cwd);
            } else {
                std::cerr << "cd: could not change to home directory\n";
            }
        } else {
            if(chdir(tokens[1].c_str()) != 0){
                std::cerr << "cd: " << tokens[1] << ": No such directory\n";
            } else {
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) != nullptr)
                    shell->updateCurrentDir(cwd);
            }
        }
        return true;
    } else if(tokens[0] == "pwd"){
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != nullptr)
            std::cout << cwd << std::endl;
        else
            std::cerr << "pwd: error getting current directory\n";
        return true;
    } else if(tokens[0] == "history"){
        std::vector<std::string>& history = shell->getHistory();
        size_t count = history.size();
        size_t start = 0;
        if(tokens.size() > 1){
            try{
                size_t n = std::stoul(tokens[1]);
                if(n < count)
                    start = count - n;
            } catch(...){
                std::cerr << "history: numeric argument required\n";
            }
        }
        for(size_t i = start; i < count; i++){
            std::cout << (i + 1) << " " << history[i] << std::endl;
        }
        return true;
    } else if(tokens[0] == "alias" || tokens[0] == "unalias"){
        return true;
    }
    return false;
}

