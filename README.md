# linux-shell

A C++ implementation of a Linux shell that is lightweight and customizable. linux-shell provides a familiar command-line interface with support for command history, aliases, and common built-in commands.

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

- A C++ compiler with C++11 support
- CMake (version 3.10 or later)
- Readline development library

On Debian/Ubuntu:
```bash
sudo apt-get install build-essential cmake libreadline-dev
```

On Arch Linux:
```bash
sudo pacman -S base-devel cmake readline
```

On Red Hat/Fedora:
```bash
sudo dnf install gcc-c++ cmake readline-devel
```

### Building and Installing

Clone the repository:
```bash
git clone https://github.com/username/linux-shell.git
cd linux-shell
```

You have two options to build and install:

### Option 1: Automated Build/Installation Script

1. Make the script executable:
```bash
chmod +x build_install.sh
```

2. Run the script:
```bash
./build_install.sh
```

### Option 2: Manual Build with CMake

1. Create a build directory, configure and build the project:
```bash
mkdir build
cd build
cmake ..
make
```

2. To install the shell system-wide (the binary will be installed to /usr/local/bin):
```bash
sudo make install
```

## Usage

After installation, launch the shell by typing:
```bash
linux-shell
```

## Command history

Command history is automatically saved to `~/.linux_shell_history` and loaded when the shell starts.

- Use the up and down arrow keys to navigate through command history.
- View history with the `history` command:
```bash
history      # Show all history
history 10   # Show the last 10 commands
```

## Aliases

Create an alias:
```bash
alias ll='ls -la'
alias gs='git status'
```

View all aliases:
```bash
alias ll='ls -la'
alias gs='git status'
```

View a specific alias:
```bash
alias ll
```

Remove an alias:
```bash
unalias ll
```

Aliases are automatically saved to `~/.linux_shell_aliases` and loaded when the shell starts.
