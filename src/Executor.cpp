#include "Executor.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <signal.h>
#include <cstdlib>

void execExternal(Shell* shell, const std::vector<std::string>& tokens) {
    pid_t pid = fork();
    if(pid == -1){
        std::cerr << "Fork failed\n";
        return;
    }
    if(pid == 0){
        // child process: restore default signal handling
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        char** args = shell->vectorToCharArray(tokens);
        execvp(args[0], args);
        std::cerr << tokens[0] << ": command not found\n";
        shell->freeCharArray(args, tokens.size());
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

