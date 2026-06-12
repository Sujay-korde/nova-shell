# NovaShell

> A Unix-like shell built from scratch in C to explore operating system fundamentals, process management, command execution, and shell internals.

[![Language](https://img.shields.io/badge/Language-C-blue.svg)]()
[![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)]()
[![Status](https://img.shields.io/badge/Status-Active%20Development-orange.svg)]()
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)]()

---

## 📖 Overview

NovaShell is a custom command-line shell developed from scratch in C as part of a deep dive into systems programming and operating system concepts.

The project aims to recreate the core functionality of modern Unix shells while providing hands-on experience with:

- Process creation and management
- System calls
- File descriptors
- Pipes and redirection
- Signal handling
- Command parsing
- Job control
- Shell architecture

Rather than treating the terminal as a black box, NovaShell explores how shells actually work internally.

---

## ✨ Features

### Current Features

- [x] Interactive shell prompt
- [x] Command parsing
- [x] External command execution
- [x] Built-in commands
- [x] Error handling

### Planned Features

- [ ] Input/Output Redirection (`>`, `<`, `>>`)
- [ ] Pipes (`|`)
- [ ] Environment Variables
- [ ] Command History
- [ ] Tab Completion
- [ ] Job Control
- [ ] Background Processes (`&`)
- [ ] Signal Handling
- [ ] Custom Configuration File
- [ ] Shell Scripting Support

---

## 🏗️ Architecture

```text
User Input
     │
     ▼
 Command Parser
     │
     ▼
 Tokenizer
     │
     ▼
 Command Dispatcher
     │
 ┌───┴──────────┐
 ▼              ▼
Built-ins    External Commands
                 │
                 ▼
             fork()
                 │
                 ▼
             execvp()
```

---

## 📂 Project Structure

```text
NovaShell/
│
├── src/
│   ├── main.c
│   ├── parser.c
│   ├── executor.c
│   ├── builtins.c
│   └── utils.c
│
├── include/
│   ├── parser.h
│   ├── executor.h
│   └── builtins.h
│
├── docs/
│
├── tests/
│
├── Makefile
├── README.md
└── LICENSE
```

---

## ⚙️ Technologies Used

- C
- GCC
- POSIX APIs
- Linux System Calls
- Make

---

## 🧠 Concepts Explored

NovaShell demonstrates understanding of:

- Operating Systems
- Systems Programming
- Process Management
- Fork-Exec Model
- Inter-Process Communication
- File Descriptors
- Unix Architecture
- Shell Design
- Memory Management

---

## 🚀 Getting Started

### Prerequisites

```bash
gcc --version
make --version
```

### Clone Repository

```bash
git clone https://github.com/YOUR_USERNAME/NovaShell.git
cd NovaShell
```

### Build

```bash
make
```

### Run

```bash
./novashell
```

---

## 💻 Example Usage

```bash
NovaShell> pwd
/home/user

NovaShell> ls
src include README.md

NovaShell> echo Hello World
Hello World

NovaShell> mkdir test
```

---

## 📈 Development Roadmap

### Phase 1 — Basic Shell

- [x] REPL Loop
- [x] Command Parsing
- [x] Process Execution

### Phase 2 — Shell Features

- [ ] Pipes
- [ ] Redirection
- [ ] Environment Variables

### Phase 3 — Advanced Features

- [ ] Job Control
- [ ] Signal Handling
- [ ] Command History

### Phase 4 — Power Features

- [ ] Shell Scripting
- [ ] Auto Completion
- [ ] Plugin System

---

## 🎯 Learning Goals

This project was created to gain practical experience with:

- Linux Internals
- POSIX Programming
- Process Scheduling
- Shell Architecture
- Systems-Level Software Development

---

## 📚 References

- The Linux Programming Interface
- Advanced Programming in the UNIX Environment
- POSIX Documentation
- Build Your Own Shell (CodeCrafters)
- Linux Man Pages

---

## 🤝 Contributing

Contributions, suggestions, and discussions are welcome.

Feel free to open an issue or submit a pull request.

---

## 📄 License

Licensed under the MIT License.

---

⭐ If you find this project interesting, consider giving it a star.
