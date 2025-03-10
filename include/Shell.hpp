#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <vector>
#include <map>

class Shell {
public:
    Shell();
    ~Shell();
    void run();
    void setupSignalHandlers();
    static void handleSignal(int signum);

    // utility functions
    std::vector<std::string> tokenize(const std::string& cmdLine);
    char** vectorToCharArray(const std::vector<std::string>& tokens);
    void freeCharArray(char** args, size_t size);
    std::string readMultiLineInput();

    // accessors for internal state
    std::string& getCurrentDir();
    std::vector<std::string>& getHistory();
    std::map<std::string, std::string>& getAliases();

    // update internal state
    void updateCurrentDir(const std::string& newDir);
    void addToHistory(const std::string& cmd);

    // delegated functions 
    bool execBuiltIn(const std::vector<std::string>& tokens);
    void execExternal(const std::vector<std::string>& tokens);
    std::string applyAlias(const std::string& cmd);
    void loadHistory();
    void saveHistory();
    void loadAliases();
    void saveAliases();

private:
    bool running;
    std::string curDir;
    std::vector<std::string> commandHistory;
    std::map<std::string, std::string> aliases;
    const size_t MAX_HISTORY_SIZE = 1000;
};

#endif // SHELL_HPP

