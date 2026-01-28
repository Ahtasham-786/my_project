# 🗂️ Smart File Management System

A modern, console-based file management system built with **C++17** to demonstrate industry-level Object-Oriented Programming, file handling, and algorithm design.

---

## 📋 Table of Contents

- [Features](#-features)
- [System Design](#-system-design)
- [Project Structure](#-project-structure)
- [Prerequisites](#-prerequisites)
- [Building the Project](#-building-the-project)
- [Usage](#-usage)
- [Learning Objectives](#-learning-objectives)
- [Code Highlights](#-code-highlights)
- [Extending the Project](#-extending-the-project)
- [Troubleshooting](#-troubleshooting)

---

## ✨ Features

### 1. **Smart File Organizer**
- Automatically sorts files into category-based folders
- Categorizes by extension: Documents, Images, Videos, Audio, Code, Archives, etc.
- Handles 50+ common file types
- Safe operation with conflict detection

### 2. **Intelligent File Search**
- Case-insensitive partial name matching
- Fast substring search algorithm
- Formatted result display with file metadata

### 3. **Duplicate File Detector**
- Hash-based duplicate detection (O(n) complexity)
- Groups duplicates for easy review
- Shows file size and path for each duplicate

### 4. **Activity Logger**
- Comprehensive logging of all operations
- Timestamped entries
- Thread-safe implementation
- Automatically created `file_manager.log`

### 5. **Interactive Menu System**
- User-friendly console interface
- Input validation and error handling
- Clear feedback for all operations

---

## 🏗️ System Design

### Architecture Pattern: **MVC (Model-View-Controller)**

```
┌─────────────────────────────────────────────────────┐
│                    Menu (Controller)                 │
│  - User interaction                                  │
│  - Input validation                                  │
│  - Coordinates operations                            │
└────────────┬─────────────────────────────────┬──────┘
             │                                 │
    ┌────────▼────────┐              ┌────────▼────────┐
    │   FileManager   │              │   FileSorter    │
    │  (Core Engine)  │              │  (Organizer)    │
    │                 │              │                 │
    │ - Scan files    │              │ - Categorize    │
    │ - Extract meta  │              │ - Move files    │
    └─────────────────┘              └─────────────────┘
             │                                 │
    ┌────────▼────────┐              ┌────────▼────────┐
    │  FileSearcher   │              │     Logger      │
    │   (Analyzer)    │              │   (Singleton)   │
    │                 │              │                 │
    │ - Search names  │              │ - Thread-safe   │
    │ - Find dupes    │              │ - Timestamped   │
    └─────────────────┘              └─────────────────┘
```

### Key Design Patterns

1. **Singleton Pattern**: Logger class (single log file instance)
2. **Dependency Injection**: Menu receives dependencies via constructor
3. **RAII**: Smart pointers for automatic memory management
4. **Facade Pattern**: FileManager simplifies `std::filesystem` API

### Class Responsibilities

| Class | Responsibility | Key Methods |
|-------|---------------|-------------|
| `FileInfo` | File metadata structure | Data holder |
| `Logger` | Activity logging (Singleton) | `log()`, `getInstance()` |
| `FileManager` | File system operations | `scanDirectory()`, `getFileInfo()` |
| `FileSorter` | File organization | `organizeByExtension()` |
| `FileSearcher` | Search & duplicate detection | `searchByName()`, `findDuplicates()` |
| `Menu` | User interface controller | `run()`, `processChoice()` |

---

## 📁 Project Structure

```
my_project/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
├── include/                 # Header files (.h)
│   ├── FileInfo.h          # File metadata structure
│   ├── Logger.h            # Logging system
│   ├── FileManager.h       # Core file operations
│   ├── FileSorter.h        # File organization
│   ├── FileSearcher.h      # Search algorithms
│   └── Menu.h              # User interface
├── src/                     # Implementation files (.cpp)
│   ├── main.cpp            # Entry point
│   ├── Logger.cpp          # Logger implementation
│   ├── FileManager.cpp     # FileManager implementation
│   ├── FileSorter.cpp      # FileSorter implementation
│   ├── FileSearcher.cpp    # FileSearcher implementation
│   └── Menu.cpp            # Menu implementation
└── test_files/              # Auto-created test directory
```

---

## 🔧 Prerequisites

### Required
- **C++17 compatible compiler**:
  - GCC 8.0+ (Linux)
  - Clang 7.0+ (macOS/Linux)
  - MSVC 2017+ (Windows)
- **CMake 3.17+**

### Installing Prerequisites

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake g++
```

#### macOS
```bash
brew install cmake
# Xcode Command Line Tools (includes clang)
xcode-select --install
```

#### Windows
- Install Visual Studio 2019+ with C++ Desktop Development
- Or install MinGW-w64 + CMake

---

## 🔨 Building the Project

### Step 1: Clone/Navigate to Project
```bash
cd /workspaces/my_project
```

### Step 2: Create Build Directory
```bash
mkdir build
cd build
```

### Step 3: Configure with CMake
```bash
# Debug build (with debugging symbols)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# OR Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Step 4: Compile
```bash
cmake --build .

# OR for verbose output
cmake --build . --verbose

# OR using make directly
make
```

### Step 5: Run
```bash
# Run with default test directory
./SmartFileManager

# OR specify custom directory
./SmartFileManager /path/to/your/files
```

---

## 🚀 Usage

### Main Menu Options

```
1️⃣  Scan Directory          - Load files from current directory
2️⃣  Organize Files          - Sort files into category folders
3️⃣  Search Files            - Find files by partial name
4️⃣  Find Duplicates         - Detect duplicate files
5️⃣  Display All Files       - Show all scanned files
6️⃣  Change Directory        - Switch to different directory
7️⃣  View Category Mappings  - See extension-to-category mapping
0️⃣  Exit                    - Quit application
```

### Typical Workflow

1. **Start the application**
   ```bash
   ./SmartFileManager
   ```

2. **Scan directory** (Option 1)
   - Loads all files from current/specified directory
   - Displays count of files found

3. **Organize files** (Option 2)
   - Creates category folders (Documents, Images, etc.)
   - Moves files to appropriate folders
   - Requires confirmation before proceeding

4. **Search for files** (Option 3)
   - Enter partial filename (e.g., "report")
   - Shows all matching files with metadata

5. **Find duplicates** (Option 4)
   - Analyzes files for duplicates
   - Groups by size + name
   - Displays all duplicate sets

6. **Check logs**
   ```bash
   cat file_manager.log
   ```

---

## 📚 Learning Objectives

This project is designed to teach:

### 1. **Object-Oriented Programming**
- ✅ Class design and encapsulation
- ✅ Inheritance and polymorphism (extensibility)
- ✅ Design patterns (Singleton, Dependency Injection)
- ✅ SOLID principles

### 2. **Modern C++ (C++17)**
- ✅ `std::filesystem` for file operations
- ✅ Smart pointers (`std::shared_ptr`, `std::make_shared`)
- ✅ Lambda expressions
- ✅ Structured bindings
- ✅ Range-based for loops

### 3. **STL Containers & Algorithms**
- ✅ `std::vector` for dynamic arrays
- ✅ `std::map` for key-value storage
- ✅ `std::transform` for data transformation
- ✅ Iterators and range-based operations

### 4. **File I/O & System Programming**
- ✅ Directory iteration
- ✅ File metadata extraction
- ✅ File moving/renaming
- ✅ Path manipulation

### 5. **Error Handling**
- ✅ Exception handling (`try-catch`)
- ✅ `std::filesystem_error` specific exceptions
- ✅ Graceful degradation
- ✅ User-friendly error messages

### 6. **Threading & Concurrency**
- ✅ `std::mutex` for thread safety
- ✅ `std::lock_guard` for RAII locking
- ✅ Singleton thread-safe initialization

### 7. **Build Systems**
- ✅ CMake configuration
- ✅ Cross-platform compilation
- ✅ Dependency management

---

## 💡 Code Highlights

### 1. Singleton Pattern (Logger)
```cpp
class Logger {
private:
    Logger();  // Private constructor
    Logger(const Logger&) = delete;  // Delete copy
    
public:
    static Logger& getInstance() {
        static Logger instance;  // Thread-safe in C++11+
        return instance;
    }
    
    void log(const std::string& message);
};
```

### 2. Modern File Iteration
```cpp
for (const auto& entry : fs::directory_iterator(targetDirectory)) {
    if (entry.is_regular_file()) {
        FileInfo info = getFileInfo(entry.path());
        files.push_back(info);
    }
}
```

### 3. Hash-Based Duplicate Detection (O(n))
```cpp
std::map<std::string, std::vector<FileInfo>> hashGroups;
for (const auto& file : files) {
    std::string hash = generateHash(file);
    hashGroups[hash].push_back(file);
}
```

### 4. RAII with Smart Pointers
```cpp
auto fileManager = std::make_shared<FileManager>(path);
auto menu = std::make_shared<Menu>(fileManager, sorter, searcher);
menu->run();
// Automatic cleanup - no manual delete needed!
```

---

## 🎯 Extending the Project

### Beginner Extensions
1. Add more file categories (Spreadsheets, Presentations)
2. Implement case-sensitive search option
3. Add file count statistics per category
4. Export search results to CSV

### Intermediate Extensions
1. **Content-based hashing**: Use MD5/SHA-256 for true duplicate detection
   ```cpp
   #include <openssl/md5.h>
   std::string computeFileHash(const fs::path& path);
   ```

2. **Recursive directory scanning**: Include subdirectories
   ```cpp
   for (const auto& entry : fs::recursive_directory_iterator(path)) {
       // Process files in all subdirectories
   }
   ```

3. **Configuration file**: JSON/XML config for custom categories
   ```cpp
   #include <nlohmann/json.hpp>
   void loadCategoriesFromJSON(const std::string& configFile);
   ```

### Advanced Extensions
1. **Multi-threading**: Parallel file processing
   ```cpp
   #include <thread>
   #include <future>
   std::vector<std::future<FileInfo>> futures;
   ```

2. **GUI Interface**: Qt or wxWidgets integration
3. **File watching**: Monitor directory changes in real-time
4. **Cloud integration**: Backup to cloud storage
5. **Compression**: Zip files by category

---

## 🐛 Troubleshooting

### Build Errors

#### Error: "filesystem: No such file or directory"
**Solution**: Ensure C++17 is enabled and compiler is recent enough
```bash
g++ --version  # Should be 8.0+
cmake -DCMAKE_CXX_STANDARD=17 ..
```

#### Error: "undefined reference to std::filesystem"
**Solution**: Link filesystem library explicitly
```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
```

### Runtime Errors

#### "Permission denied" when organizing files
**Solution**: Run with appropriate permissions
```bash
# Linux/Mac
sudo ./SmartFileManager

# Or change directory ownership
sudo chown -R $USER:$USER /path/to/directory
```

#### "Directory does not exist"
**Solution**: Verify path and create if needed
```bash
mkdir -p /path/to/directory
./SmartFileManager /path/to/directory
```

### CMake Issues

#### CMake version too old
```bash
# Ubuntu
sudo apt-add-repository ppa:george-edison55/cmake-3.x
sudo apt update
sudo apt install cmake

# Or install from source
wget https://github.com/Kitware/CMake/releases/download/v3.25.0/cmake-3.25.0.tar.gz
tar -xzf cmake-3.25.0.tar.gz
cd cmake-3.25.0
./bootstrap && make && sudo make install
```

---

## 📖 Additional Resources

### C++ References
- [cppreference.com](https://en.cppreference.com/) - Complete C++ reference
- [std::filesystem](https://en.cppreference.com/w/cpp/filesystem) - Filesystem library docs
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - Best practices

### Books
- "The C++ Programming Language" by Bjarne Stroustrup
- "Effective Modern C++" by Scott Meyers
- "C++ Primer" by Stanley Lippman

### Online Courses
- Coursera: C++ For C Programmers
- Udemy: Beginning C++ Programming
- edX: Introduction to C++

---

## 🤝 Contributing

This is an educational project. Feel free to:
- Fork and experiment
- Add new features
- Submit improvements
- Share your learning journey

---

## 📝 License

This project is created for educational purposes. Feel free to use and modify for learning.

---

## 👨‍🏫 Author's Note

This project represents a **complete, production-quality C++ application** designed specifically for learning. Every line of code includes educational comments explaining:
- **What** the code does
- **Why** design decisions were made
- **How** concepts relate to industry practices

Take your time to read through the code, experiment with modifications, and most importantly - **build something of your own**!

The best way to master C++ is through hands-on practice. Use this project as a foundation, then expand it with your own ideas.

**Happy coding! 🚀**

---

## 📊 Project Statistics

- **Total Lines of Code**: ~2,500+ (with comments)
- **Classes**: 6 (+ 1 struct)
- **Design Patterns**: 3 (Singleton, Dependency Injection, MVC)
- **STL Containers Used**: 2 (vector, map)
- **File Operations**: 10+ (read, write, move, scan, etc.)
- **C++17 Features**: 8+ (filesystem, structured bindings, auto, etc.)

---

**Built with ❤️ for C++ learners aiming for industry expertise**