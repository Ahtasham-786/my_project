#include <iostream>
#include <memory>
#include <filesystem>
#include "FileManager.h"
#include "FileSorter.h"
#include "FileSearcher.h"
#include "Menu.h"
#include "Logger.h"

/**
 * =============================================================================
 * MAIN.CPP - APPLICATION ENTRY POINT
 * =============================================================================
 * 
 * This is where everything comes together!
 * 
 * PURPOSE OF MAIN():
 * 1. Initialize all core components
 * 2. Set up dependencies
 * 3. Launch the application
 * 4. Handle top-level errors
 * 
 * Teaching Point: main() should be SIMPLE
 * - Create objects
 * - Call main function
 * - Handle errors
 * - Return status code
 * 
 * All complex logic should be in classes (as we've done).
 * main() is just the bootstrap/launcher.
 */

namespace fs = std::filesystem;

/**
 * @brief Gets default directory to manage
 * @return Path to directory
 * 
 * Teaching Point: DEFAULT VALUE STRATEGY
 * 
 * OPTIONS:
 * 1. Current directory (std::filesystem::current_path())
 * 2. User home directory
 * 3. Test directory (for development)
 * 4. Command-line argument
 * 
 * Here we use current directory + test folder for safety
 */
std::string getDefaultDirectory() {
    /**
     * Teaching Point: std::filesystem::current_path()
     * 
     * Returns the directory where program is RUN from
     * NOT where executable is located!
     * 
     * Example:
     * Executable at: /usr/bin/myapp
     * Run from: /home/user/documents
     * current_path() returns: /home/user/documents
     */
    fs::path current = fs::current_path();
    
    /**
     * Teaching Point: PATH CONCATENATION
     * 
     * fs::path supports / operator for concatenation
     * Automatically handles platform-specific separators:
     * - Windows: \
     * - Unix/Linux: /
     * 
     * This is why std::filesystem is so powerful!
     */
    fs::path testDir = current / "test_files";
    
    // Create test directory if it doesn't exist
    if (!fs::exists(testDir)) {
        try {
            fs::create_directory(testDir);
            std::cout << "📁 Created test directory: " << testDir << "\n";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "⚠️  Could not create test directory: " << e.what() << "\n";
            return current.string();  // Fall back to current directory
        }
    }
    
    return testDir.string();
}

/**
 * @brief Main entry point
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit code (0 = success, non-zero = error)
 * 
 * Teaching Point: COMMAND-LINE ARGUMENTS
 * 
 * int main(int argc, char* argv[])
 * 
 * argc = Argument Count (number of arguments)
 * argv = Argument Vector (array of C-strings)
 * 
 * Example: ./program /home/user/files
 * argc = 2
 * argv[0] = "./program" (program name)
 * argv[1] = "/home/user/files" (first argument)
 * 
 * We use this to allow specifying directory:
 * ./file_manager /path/to/directory
 */
int main(int argc, char* argv[]) {
    try {
        /**
         * Teaching Point: STARTUP SEQUENCE
         * 
         * Good practice:
         * 1. Display welcome message
         * 2. Initialize logger first (so everything can log)
         * 3. Parse arguments
         * 4. Validate configuration
         * 5. Create core objects
         * 6. Start application
         */
        
        // Step 1: Welcome message
        std::cout << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════╗\n";
        std::cout << "║                                                          ║\n";
        std::cout << "║        SMART FILE MANAGEMENT SYSTEM                      ║\n";
        std::cout << "║        Modern C++17 Solution                             ║\n";
        std::cout << "║                                                          ║\n";
        std::cout << "║        Features:                                         ║\n";
        std::cout << "║        • Smart file organization by extension            ║\n";
        std::cout << "║        • Intelligent file search                         ║\n";
        std::cout << "║        • Duplicate file detection                        ║\n";
        std::cout << "║        • Comprehensive activity logging                  ║\n";
        std::cout << "║                                                          ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        
        // Step 2: Initialize logger (singleton - only logs initialization)
        Logger::getInstance().log("=== Application Starting ===");
        Logger::getInstance().log("Smart File Management System v1.0");
        
        // Step 3: Parse command-line arguments
        std::string targetDirectory;
        
        if (argc > 1) {
            // User provided directory as argument
            targetDirectory = argv[1];
            std::cout << "📂 Using directory from argument: " << targetDirectory << "\n\n";
        } else {
            // No argument, use default
            targetDirectory = getDefaultDirectory();
            std::cout << "📂 Using default directory: " << targetDirectory << "\n";
            std::cout << "   (You can specify a directory: ./program /path/to/dir)\n\n";
        }
        
        Logger::getInstance().log("Target directory: " + targetDirectory);
        
        /**
         * Teaching Point: OBJECT CREATION WITH SMART POINTERS
         * 
         * std::make_shared<T>(args...)
         * 
         * Creates shared_ptr<T> efficiently
         * Benefits over new:
         * - Single memory allocation (more efficient)
         * - Exception-safe
         * - Automatic cleanup
         * 
         * ALTERNATIVE:
         * auto ptr = std::shared_ptr<FileManager>(new FileManager(path));
         * 
         * But make_shared is preferred (more efficient + safer)
         */
        
        // Step 4: Create core components
        std::cout << "⚙️  Initializing components...\n";
        
        auto fileManager = std::make_shared<FileManager>(targetDirectory);
        auto fileSorter = std::make_shared<FileSorter>();
        auto fileSearcher = std::make_shared<FileSearcher>();
        
        /**
         * Teaching Point: DEPENDENCY INJECTION IN ACTION
         * 
         * We create dependencies OUTSIDE of Menu
         * Then pass them TO Menu via constructor
         * 
         * This is Inversion of Control (IoC)
         * - Menu doesn't create its dependencies
         * - main() creates and injects them
         * 
         * Benefits:
         * - main() controls object lifetime
         * - Easy to change implementations
         * - Easy to add logging/monitoring wrappers
         * - Testability (can inject mocks)
         */
        auto menu = std::make_shared<Menu>(fileManager, fileSorter, 
                                          fileSearcher, targetDirectory);
        
        std::cout << "✅ Initialization complete!\n";
        std::cout << "\nPress Enter to start...";
        std::cin.get();
        
        // Step 5: Start application (blocking call - runs until user exits)
        menu->run();
        
        /**
         * Teaching Point: CLEANUP
         * 
         * No explicit cleanup needed!
         * When main() exits:
         * 1. All shared_ptr go out of scope
         * 2. Reference counts reach zero
         * 3. Objects deleted automatically
         * 4. Destructors called (Logger closes file, etc.)
         * 
         * This is RAII in action - Resource Acquisition Is Initialization
         * Resource lifetime tied to object lifetime
         * No manual new/delete needed
         * Exception-safe (cleanup happens even with exceptions)
         */
        
        return 0;  // Success exit code
        
    } catch (const fs::filesystem_error& e) {
        /**
         * Teaching Point: EXCEPTION HIERARCHY
         * 
         * Catch most specific exceptions first
         * Then catch more general ones
         * 
         * Here:
         * 1. filesystem_error (specific)
         * 2. exception (general)
         */
        std::cerr << "\n❌ Filesystem Error: " << e.what() << "\n";
        std::cerr << "   Path 1: " << e.path1() << "\n";
        if (!e.path2().empty()) {
            std::cerr << "   Path 2: " << e.path2() << "\n";
        }
        Logger::getInstance().log("FATAL: Filesystem error - " + std::string(e.what()));
        return 1;  // Error exit code
        
    } catch (const std::exception& e) {
        /**
         * Teaching Point: CATCH-ALL FOR STANDARD EXCEPTIONS
         * 
         * std::exception is base for all standard exceptions:
         * - std::runtime_error
         * - std::logic_error
         * - std::bad_alloc (out of memory)
         * - std::overflow_error
         * - etc.
         */
        std::cerr << "\n❌ Error: " << e.what() << "\n";
        Logger::getInstance().log("FATAL: " + std::string(e.what()));
        return 1;
        
    } catch (...) {
        /**
         * Teaching Point: CATCH ABSOLUTELY EVERYTHING
         * 
         * catch (...) catches ANY exception, even non-standard ones
         * 
         * This is a last resort safety net
         * Prevents program from crashing without any error message
         * 
         * Use sparingly - you can't access exception details here
         */
        std::cerr << "\n❌ Unknown error occurred!\n";
        Logger::getInstance().log("FATAL: Unknown exception");
        return 2;  // Different code for unknown errors
    }
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM MAIN.CPP
 * =============================================================================
 * 
 * 1. MAIN() RESPONSIBILITY:
 *    - Bootstrap/launcher only
 *    - Complex logic in classes
 *    - Simple and readable
 * 
 * 2. SMART POINTERS:
 *    - std::make_shared for creation
 *    - Automatic memory management
 *    - RAII for cleanup
 *    - No manual new/delete
 * 
 * 3. DEPENDENCY INJECTION:
 *    - Create dependencies outside
 *    - Inject via constructor
 *    - Loose coupling
 *    - Testability
 * 
 * 4. EXCEPTION HANDLING:
 *    - Specific exceptions first
 *    - General exceptions next
 *    - Catch-all as last resort
 *    - Different exit codes for different errors
 * 
 * 5. COMMAND-LINE ARGUMENTS:
 *    - argc/argv parsing
 *    - Default values
 *    - User-friendly feedback
 * 
 * 6. INITIALIZATION SEQUENCE:
 *    - Welcome message
 *    - Logger first
 *    - Parse arguments
 *    - Create objects
 *    - Start application
 * 
 * 7. ERROR CODES:
 *    - 0 = Success
 *    - 1 = Known error
 *    - 2 = Unknown error
 *    - Allows scripts to detect failures
 * 
 * INDUSTRY BEST PRACTICES:
 * - RAII for all resources
 * - Smart pointers everywhere
 * - Comprehensive exception handling
 * - Clean separation of concerns
 * - Defensive programming
 * - Clear user communication
 * - Proper exit codes for scripting
 * 
 * =============================================================================
 * CONGRATULATIONS! 🎉
 * =============================================================================
 * 
 * You've now seen a complete, production-quality C++ application featuring:
 * 
 * ✅ Object-Oriented Design (Classes, Encapsulation, Abstraction)
 * ✅ Design Patterns (Singleton, Dependency Injection, MVC)
 * ✅ Modern C++17 Features (filesystem, auto, smart pointers, lambdas)
 * ✅ STL Mastery (vector, map, algorithms, iterators)
 * ✅ Error Handling (Exception hierarchy, graceful degradation)
 * ✅ File Operations (Reading, moving, organizing files)
 * ✅ Algorithms (Search, duplicate detection, sorting)
 * ✅ Best Practices (RAII, const correctness, separation of concerns)
 * 
 * NEXT STEPS FOR MASTERY:
 * 
 * 1. BUILD & RUN: Compile and test the application
 * 2. EXPERIMENT: Modify features, add new ones
 * 3. DEBUG: Use debugger to step through code
 * 4. EXTEND: Add features like:
 *    - Content-based file hashing (MD5/SHA-256)
 *    - Recursive directory scanning
 *    - File size filtering
 *    - Date-based organization
 *    - Configuration file support
 *    - Multi-threading for large directories
 * 5. OPTIMIZE: Profile and improve performance
 * 6. TEST: Write unit tests (Google Test framework)
 * 
 * Remember: The best way to learn is by doing!
 * Modify this code, break it, fix it, and make it your own.
 * 
 * Good luck on your C++ journey! 🚀
 * =============================================================================
 */
