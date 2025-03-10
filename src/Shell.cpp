#include "Shell.hpp"
#include "BuiltIn.hpp"
#include "History.hpp"
#include "Alias.hpp"
#include "Executor.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <signal.h>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

// Global pointer for signal handling
static Shell* globalShellPtr = nullptr;

Shell::Shell() : running(true) {
    // initialize current directory
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != nullptr) {
        curDir = cwd;
    } else {
        curDir = "/";
    }
    // enable tab completion via readline
    rl_bind_key('\t', rl_complete);
    setupSignalHandlers();
    loadHistory();
    loadAliases();
    globalShellPtr = this;
}

Shell::~Shell(){
    saveHistory();
    saveAliases();
}

void Shell::setupSignalHandlers(){
    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);
}

void Shell::handleSignal(int signum){
    if(globalShellPtr) {
        switch(signum) {
            case SIGINT:
                std::cout << "\n";
                if(globalShellPtr->running){
                    std::cout << globalShellPtr->curDir << "$ ";
                    std::cout.flush();
                }
                break;
            case SIGTSTP:
                // Ignore Ctrl+Z for this shell
                std::cout << "\n";
                if(globalShellPtr->running){
                    std::cout << globalShellPtr->curDir << "$ ";
                    std::cout.flush();
                }
                break;
        }
    }
}

std::vector<std::string> Shell::tokenize(const std::string& cmdLine){
    std::vector<std::string> tokens;
    std::string token;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    for (size_t i = 0; i < cmdLine.length(); ++i){
        char c = cmdLine[i];
        if(c == '\'' && !inDoubleQuote){
            inSingleQuote = !inSingleQuote;
            continue;
        } else if(c == '"' && !inSingleQuote){
            inDoubleQuote = !inDoubleQuote;
            continue;
        }
        if(c == ' ' && !inSingleQuote && !inDoubleQuote){
            if(!token.empty()){
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if(!token.empty()) tokens.push_back(token);
    if(inSingleQuote || inDoubleQuote)
        std::cerr << "Error: Unclosed quotes in command\n";
    return tokens;
}

char** Shell::vectorToCharArray(const std::vector<std::string>& tokens){
    char** args = new char*[tokens.size() + 1];
    for (size_t i = 0; i < tokens.size(); i++){
        args[i] = new char[tokens[i].length() + 1];
        strcpy(args[i], tokens[i].c_str());
    }
    args[tokens.size()] = nullptr;
    return args;
}

void Shell::freeCharArray(char** args, size_t size){
    for (size_t i = 0; i < size; i++){
        delete[] args[i];
    }
    delete[] args;
}

std::string Shell::readMultiLineInput(){
    std::string completeCommand;
    char* input;
    bool continueReading = true;
    std::string prompt = curDir + "$ ";
    while(continueReading) {
        input = readline(prompt.c_str());
        if(!input){
            std::cout << "exit\n";
            running = false;
            return "";
        }
        std::string line = input;
        free(input);
        if(!line.empty() && line.back() == '\\'){
            line.pop_back();
            completeCommand += line;
            prompt = "> ";
        } else {
            completeCommand += line;
            continueReading = false;
        }
    }
    return completeCommand;
}

std::string& Shell::getCurrentDir(){
    return curDir;
}

std::vector<std::string>& Shell::getHistory(){
    return commandHistory;
}

std::map<std::string, std::string>& Shell::getAliases(){
    return aliases;
}

void Shell::updateCurrentDir(const std::string& newDir){
    curDir = newDir;
}

void Shell::addToHistory(const std::string& cmd){
    if(cmd.empty() || (!commandHistory.empty() && cmd == commandHistory.back()))
        return;
    commandHistory.push_back(cmd);
    add_history(cmd.c_str());
    if(commandHistory.size() > MAX_HISTORY_SIZE)
        commandHistory.erase(commandHistory.begin());
}

bool Shell::execBuiltIn(const std::vector<std::string>& tokens){
    return ::execBuiltIn(this, tokens);
}

void Shell::execExternal(const std::vector<std::string>& tokens){
    ::execExternal(this, tokens);
}

std::string Shell::applyAlias(const std::string& cmd){
    return ::applyAlias(this, cmd);
}

void Shell::loadHistory(){
    ::loadHistory(this);
}

void Shell::saveHistory(){
    ::saveHistory(this);
}

void Shell::loadAliases(){
    ::loadAliases(this);
}

void Shell::saveAliases(){
    ::saveAliases(this);
}

void Shell::run(){
    while(running){
        std::string cmdLine = readMultiLineInput();
        if(cmdLine.empty() || !running)
            continue;
        addToHistory(cmdLine);
        cmdLine = applyAlias(cmdLine);
        std::vector<std::string> tokens = tokenize(cmdLine);
        if(!tokens.empty()){
            if(!execBuiltIn(tokens))
                execExternal(tokens);
        }
    }
    saveHistory();
    saveAliases();
}

