# MyShell

A lightweight and customizable Linux shell implementation written in C++. MyShell provides a familiar command-line interface with support for command history, aliases, and common built-in commands.

## Features

- **Command Execution**: Run any standard Linux programs and commands
- **Built-in Commands**: 
  - `cd`: Change directory
  - `pwd`: Print working directory
  - `echo`: Display text
  - `history`: View command history
  - `alias`/`unalias`: Create and manage command aliases
  - `exit`: Exit the shell
- **Command History**: Persistent history between sessions with support for history navigation
- **Tab Completion**: File and command tab completion
- **Aliases**: Define custom command shortcuts that persist between sessions
- **Signal Handling**: Properly handles Ctrl+C (SIGINT) and Ctrl+Z (SIGTSTP)
- **Multiline Commands**: Support for command continuation with backslash

## Installation

### Prerequisites
- C++ compiler with C++11 support
- readline development library

On Debian/Ubuntu systems:
```bash
sudo apt-get install build-essential libreadline-dev
```

On Arch Linux systems:
```bash
sudo pacman -S base-devel readline
```

On Red Hat/Fedora systems:
```bash
sudo dnf install gcc-c++ readline-devel
```

### Building
```bash
git clone https://github.com/username/linux-shell.git
cd linux-shell
g++ -o myshell main.cpp -lreadline
```

## Usage

Launch the shell:
```bash
./myshell
```

### Command History
Command history is automatically saved to `~/.myshell_history` and loaded when the shell starts.

Use the up and down arrow keys to navigate through command history.

View history with the `history` command:
```
history      # Show all history
history 10   # Show last 10 commands
```

### Aliases
Create an alias:
```
alias ll='ls -la'
alias gs='git status'
```

View all aliases:
```
alias
```

View a specific alias:
```
alias ll
```

Remove an alias:
```
unalias ll
```

Aliases are automatically saved to `~/.myshell_aliases` and loaded when the shell starts.
