#ifndef MENU_H
#define MENU_H

#include <memory>
#include "FileManager.h"
#include "FileSorter.h"
#include "FileSearcher.h"

/**
 * @brief Menu Class - Console User Interface Controller
 * 
 * RESPONSIBILITY: Manages user interaction and application flow
 * 
 * DESIGN PATTERN: Controller (from MVC pattern)
 * - Model: FileManager, FileSorter, FileSearcher (business logic)
 * - View: Console output (display methods)
 * - Controller: Menu (this class - coordinates model and view)
 * 
 * OOP PRINCIPLE: Dependency Injection
 * Menu depends on FileManager, FileSorter, FileSearcher, but receives them
 * as constructor parameters rather than creating them internally.
 * Benefits:
 * - Testability: Can inject mock objects for testing
 * - Flexibility: Can swap implementations without changing Menu
 * - Loose coupling: Menu doesn't know HOW objects are created
 * 
 * Teaching Point: This separation of concerns makes code maintainable.
 * If we want to add GUI later, we only replace Menu, not core logic.
 */
class Menu {
private:
    /**
     * @brief Smart pointers to core components
     * 
     * Teaching Point: std::shared_ptr vs raw pointers
     * 
     * RAW POINTER: FileManager* manager;
     * Problems:
     * - Manual memory management (new/delete)
     * - Memory leaks if forget to delete
     * - Dangling pointers after delete
     * 
     * SHARED_PTR: std::shared_ptr<FileManager>
     * Benefits:
     * - Automatic memory management (RAII)
     * - Reference counting (deletes when last reference gone)
     * - Exception-safe (no leaks even with exceptions)
     * - Clear ownership semantics
     * 
     * Why shared_ptr not unique_ptr?
     * - Might want to pass these objects to other functions
     * - Shared ownership model fits better here
     */
    std::shared_ptr<FileManager> fileManager;
    std::shared_ptr<FileSorter> fileSorter;
    std::shared_ptr<FileSearcher> fileSearcher;
    
    std::string currentDirectory;   // Working directory path
    bool isRunning;                 // Application state flag
    
    /**
     * @brief Displays main menu options
     * 
     * Teaching Point: Const method - doesn't modify object state.
     * Display methods should always be const (read-only operations).
     */
    void displayMainMenu() const;
    
    /**
     * @brief Processes user's menu choice
     * @param choice Menu option number
     * 
     * Teaching Point: Switch statement for menu routing.
     * Alternative: Command pattern with map<int, function>
     * Trade-off: switch is simpler, command pattern more extensible
     */
    void processChoice(int choice);
    
    /**
     * @brief Menu option handlers (one per feature)
     * 
     * Teaching Point: Each handler is a separate method (Single Responsibility).
     * This makes code:
     * - Easier to test (test each feature independently)
     * - Easier to maintain (modify one feature without affecting others)
     * - Easier to understand (each method has clear purpose)
     */
    void handleScanDirectory();
    void handleOrganizeFiles();
    void handleSearchFiles();
    void handleFindDuplicates();
    void handleDisplayFiles();
    void handleChangeDirectory();
    
    /**
     * @brief Utility methods for user interaction
     */
    void pauseScreen() const;           // Wait for user keypress
    void clearScreen() const;           // Clear console
    std::string getUserInput(const std::string& prompt) const;  // Get string input
    int getIntInput(const std::string& prompt) const;           // Get integer input

public:
    /**
     * @brief Constructor with dependency injection
     * @param manager FileManager instance
     * @param sorter FileSorter instance  
     * @param searcher FileSearcher instance
     * @param directory Initial working directory
     * 
     * Teaching Point: Constructor receives fully-constructed dependencies.
     * This is Dependency Injection - objects are "injected" rather than created.
     * 
     * Benefits:
     * 1. Testability: Can inject mock objects for unit testing
     * 2. Flexibility: Can use different implementations
     * 3. Clear dependencies: All requirements visible in constructor signature
     */
    Menu(std::shared_ptr<FileManager> manager,
         std::shared_ptr<FileSorter> sorter,
         std::shared_ptr<FileSearcher> searcher,
         const std::string& directory);
    
    /**
     * @brief Starts the main application loop
     * 
     * Teaching Point: Main event loop pattern
     * 
     * ALGORITHM:
     * 1. While isRunning is true:
     *    a. Display menu
     *    b. Get user choice
     *    c. Process choice
     *    d. Handle errors gracefully
     * 2. Exit when user chooses quit option
     * 
     * This is the same pattern used in:
     * - Game engines (game loop)
     * - GUI frameworks (event loop)
     * - Server applications (request loop)
     */
    void run();
    
    /**
     * @brief Exits the application
     * 
     * Teaching Point: Proper shutdown sequence
     * - Set isRunning to false (stops main loop)
     * - Log shutdown event
     * - Cleanup happens automatically (RAII with smart pointers)
     */
    void exit();
};

#endif // MENU_H
