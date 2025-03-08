#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <map>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

class Shell;
Shell* globalShellPtr = nullptr;

class Shell{
private:
  bool running;
  std::string curDir;
  std::vector<std::string> commandHistory;
  std::map<std::string, std::string> aliases;
  const size_t MAX_HISTORY_SIZE = 1000;
  const std::string HISTORY_FILE = std::string(getenv("HOME")) + "/.myshell_history";
  const std::string ALIAS_FILE = std::string(getenv("HOME")) + "/.myshell_aliases";
  
  // split command into tokens
  std::vector<std::string> tokenize(const std::string& cmdLine){
    std::vector<std::string> tokens;
    std::string token;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    for (size_t i = 0; i < cmdLine.length(); ++i){
      char c = cmdLine[i];
      // handle quotes
      if (c == '\'' && !inDoubleQuote){
        inSingleQuote = !inSingleQuote;
        continue;
      } else if (c == '"' && !inSingleQuote){
        inDoubleQuote = !inDoubleQuote;
        continue;
      }
      // handle spaces as token separators when not in quotes
      if (c == ' ' && !inSingleQuote && !inDoubleQuote){
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
      } else {
        token += c;
      }
    }
    if (!token.empty()) tokens.push_back(token);
    if (inSingleQuote || inDoubleQuote) std::cerr << "Error: Unclosed quotes in command\n"; 
    return tokens;
  }
  
  // convert vector of string tokens to char array for execvp
  char** vectorToCharArray(const std::vector<std::string>& tokens){
    char** args = new char*[tokens.size() + 1];
    for (size_t i = 0; i < tokens.size(); i++){
      args[i] = new char[tokens[i].length() + 1];
      strcpy(args[i], tokens[i].c_str());
    }
    args[tokens.size()] = nullptr;
    return args;
  }
  
  // free memory alloc for char* array
  void freeCharArray(char** args, size_t size){
    for (size_t i = 0; i < size; i++) delete[] args[i];
    delete[] args;
  }
  
  // load command history from file
  void loadHistory(){
    std::ifstream historyFile(HISTORY_FILE);
    if (historyFile.is_open()){
      std::string line;
      while (std::getline(historyFile, line)){
        commandHistory.push_back(line);
        add_history(line.c_str()); 
      }
      historyFile.close();
    }
  }
  
  // save command history to file
  void saveHistory(){
    std::ofstream historyFile(HISTORY_FILE);
    if (historyFile.is_open()){
      for (const auto& cmd : commandHistory) historyFile << cmd << std::endl;
      historyFile.close();
    }
  }
  
  // add command to history
  void addHistory(const std::string& cmd){
    if (cmd.empty() || (commandHistory.size() > 0 && cmd == commandHistory.back())){
      return;
    }
    commandHistory.push_back(cmd);
    add_history(cmd.c_str()); 
    
    if (commandHistory.size() > MAX_HISTORY_SIZE) commandHistory.erase(commandHistory.begin());
  }
  
  // load aliases from file
  void loadAliases(){
    std::ifstream aliasFile(ALIAS_FILE);
    if (aliasFile.is_open()){
      std::string line;
      while (std::getline(aliasFile, line)){
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
          std::string alias = line.substr(0, pos);
          std::string command = line.substr(pos + 1);
          aliases[alias] = command;
        }
      }
      aliasFile.close();
    }
  }
  
  // save aliases to file
  void saveAliases(){
    std::ofstream aliasFile(ALIAS_FILE);
    if (aliasFile.is_open()){
      for (const auto& pair : aliases) aliasFile << pair.first << "=" << pair.second << std::endl;
      aliasFile.close();
    }
  }
  
  // alias substitution
  std::string applyAlias(const std::string& cmd){
    std::vector<std::string> tokens = tokenize(cmd);
    if (tokens.empty()) return cmd;
    
    auto it = aliases.find(tokens[0]);
    if (it != aliases.end()){
      std::string result = it->second;
      for (size_t i = 1; i < tokens.size(); i++){
        result += " " + tokens[i];
      }
      return result;
    }
    return cmd;
  }
  
  // execute builtin commands
  bool execBuiltIn(const std::vector<std::string>& tokens){
    if (tokens.empty()) return true;

    if (tokens[0] == "exit"){
      running = false;
      return true;
    } else if (tokens[0] == "echo"){
      // echo command
      for (size_t i = 1; i < tokens.size(); ++i){
        if (i > 1) std::cout << " ";
        std::cout << tokens[i];
      }
      std::cout << std::endl;
      return true;
    } else if (tokens[0] == "cd"){
      if (tokens.size() < 2) {
        const char* homeDir = getenv("HOME");
        if (homeDir && chdir(homeDir) == 0){
          char cwd[1024];
          if (getcwd(cwd, sizeof(cwd)) != nullptr) curDir = cwd;
        } else {
          std::cerr << "cd: could not change to home directory\n";
        }
      } else {
        if (chdir(tokens[1].c_str()) != 0){
          std::cerr << "cd: " << tokens[1] << ": No such directory\n";
        } else {
          char cwd[1024];
          if (getcwd(cwd, sizeof(cwd)) != nullptr){
            curDir = cwd;
          }
        }
      }
      return true;
    } else if (tokens[0] == "pwd"){
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != nullptr){
        std::cout << cwd << std::endl;
      } else {
        std::cerr << "pwd: error getting current directory\n";
      }
      return true;
    } else if (tokens[0] == "history"){
      // command history
      size_t count = commandHistory.size();
      size_t start = 0;
      if (tokens.size() > 1){
        try {
          size_t n = std::stoul(tokens[1]);
          if (n < count) {
            start = count - n;
          }
        } catch (...){
          std::cerr << "history: numeric argument required\n";
        }
      }
      for (size_t i = start; i < count; i++) std::cout << (i + 1) << " " << commandHistory[i] << std::endl;
      return true;
    } else if (tokens[0] == "alias"){
      if (tokens.size() == 1){
        // list all aliases
        for (const auto& pair : aliases) std::cout << "alias " << pair.first << "='" << pair.second << "'" << std::endl;
      } else if (tokens.size() >= 2){
        // parse alias definition
        std::string aliasStr = tokens[1];
        size_t pos = aliasStr.find('=');

        if (pos == std::string::npos && tokens.size() == 2) {
          // show specific alias
          auto it = aliases.find(aliasStr);
          if (it != aliases.end()) {
            std::cout << "alias " << it->first << "='" << it->second << "'" << std::endl;
          } else {
            std::cerr << "alias: " << aliasStr << ": not found\n";
          }
        } else {
          // define new alias
          std::string alias, command;
          if (pos != std::string::npos){
            alias = aliasStr.substr(0, pos);
            command = aliasStr.substr(pos + 1);
            if ((command.front() == '\'' && command.back() == '\'') ||
                (command.front() == '"' && command.back() == '"')) {
              command = command.substr(1, command.length() - 2);
            }
          } else {
            alias = tokens[1];
            command = "";
            for (size_t i = 2; i < tokens.size(); i++) {
              if (i > 2) command += " ";
              command += tokens[i];
            }
          }
          aliases[alias] = command;
          saveAliases();
        }
      }
      return true;
    } else if (tokens[0] == "unalias"){
      if (tokens.size() == 2){
        auto it = aliases.find(tokens[1]);
        if (it != aliases.end()){
          aliases.erase(it);
          saveAliases();
        } else {
          std::cerr << "unalias: " << tokens[1] << ": not found\n";
        }
      } else {
        std::cerr << "unalias: usage: unalias name\n";
      }
      return true;
    }
    return false;
  }
  
  // execute external commands
  void execExternal(const std::vector<std::string>& tokens){
    pid_t pid = fork();
    if (pid == -1){
      std::cerr << "Fork failed\n";
      return;
    }
    if (pid == 0){
      // child process
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      
      char** args = vectorToCharArray(tokens);
      execvp(args[0], args);
      std::cerr << tokens[0] << ": command not found\n";
      freeCharArray(args, tokens.size());
      exit(1);
    } else {
      // parent process
      int status;
      waitpid(pid, &status, 0);
    }
  }

  // read multiline input with continuation character '\'
  std::string readMultiLineInput(){
    std::string completeCommand;
    char* input;
    bool continueReading = true;
    std::string prompt = curDir + "$ ";
    
    while (continueReading){
      input = readline(prompt.c_str());
      
      if (!input) {
        // EOF (ctrl+d) - exit shell
        std::cout << "exit\n";
        running = false;
        return "";
      }
      std::string line = input;
      free(input);
      // check if line ends with continuation character
      if (!line.empty() && line.back() == '\\'){
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
  
public:
  Shell(): running(true){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr){
      curDir = cwd;
    } else {
      curDir = "/";
    }
    // initialize readline with custom settings
    rl_bind_key('\t', rl_complete);  // enable tab completion
    setupSignalHandlers();
    loadHistory();
    loadAliases();
    globalShellPtr = this;
  }
  
  ~Shell(){
    saveHistory();
    saveAliases();
  }
  
  void setupSignalHandlers(){
    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);
  }
  
  static void handleSignal(int signum){
    if (globalShellPtr){
      switch (signum){
        case SIGINT:
          std::cout << "\n";
          if (globalShellPtr->running){
            std::cout << globalShellPtr->curDir << "$ ";
            std::cout.flush();
          }
          break;
        case SIGTSTP:
          // ignore ctrl+z for shell
          std::cout << "\n";
          if (globalShellPtr->running){
            std::cout << globalShellPtr->curDir << "$ ";
            std::cout.flush();
          }
          break;
      }
    }
  }
  
  void run(){
    while (running){
      std::string cmdLine = readMultiLineInput();
      
      if (cmdLine.empty() || !running) continue;
      
      addHistory(cmdLine);
      cmdLine = applyAlias(cmdLine);
      std::vector<std::string> tokens = tokenize(cmdLine);
      if (!tokens.empty()){
        if (!execBuiltIn(tokens)) execExternal(tokens);
      }
    } 
    saveHistory();
    saveAliases();
  }
};

int main(){
  Shell shell;
  shell.run();
  return 0;
}
