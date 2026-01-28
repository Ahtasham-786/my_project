#include "../include/Menu.h"
#include "../include/Logger.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <algorithm>

/**
 * =============================================================================
 * MENU IMPLEMENTATION - MVC CONTROLLER PATTERN
 * =============================================================================
 * 
 * This class demonstrates:
 * 1. Controller pattern (from MVC architecture)
 * 2. Event loop implementation
 * 3. User input validation
 * 4. Dependency injection
 * 5. Exception handling for user interaction
 */

/**
 * @brief Constructor with dependency injection
 * 
 * Teaching Point: DEPENDENCY INJECTION PATTERN
 * 
 * TIGHT COUPLING (Bad):
 * Menu::Menu() {
 *     fileManager = new FileManager("/some/path");  // Menu creates dependencies
 * }
 * 
 * LOOSE COUPLING (Good - used here):
 * Menu::Menu(shared_ptr<FileManager> manager) {
 *     fileManager = manager;  // Menu receives dependencies
 * }
 * 
 * BENEFITS:
 * 1. Testability: Can inject mock objects for unit tests
 * 2. Flexibility: Can swap implementations
 * 3. Separation: Menu doesn't know HOW to create dependencies
 * 4. Reusability: Same Menu can work with different managers
 */
Menu::Menu(std::shared_ptr<FileManager> manager,
           std::shared_ptr<FileSorter> sorter,
           std::shared_ptr<FileSearcher> searcher,
           const std::string& directory)
    : fileManager(manager),
      fileSorter(sorter),
      fileSearcher(searcher),
      currentDirectory(directory),
      isRunning(true) {
    
    Logger::getInstance().log("Menu system initialized");
}

/**
 * @brief Main application loop
 * 
 * Teaching Point: EVENT LOOP PATTERN
 * 
 * This is the same pattern used in:
 * - Game engines (game loop)
 * - GUI frameworks (event loop)
 * - Web servers (request loop)
 * 
 * STRUCTURE:
 * while (isRunning) {
 *     1. Display UI
 *     2. Get user input
 *     3. Process input
 *     4. Handle errors
 * }
 * 
 * TERMINATION:
 * Loop continues until isRunning becomes false
 * User chooses "Exit" → processChoice() sets isRunning = false
 */
void Menu::run() {
    Logger::getInstance().log("Application started");
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║       SMART FILE MANAGEMENT SYSTEM v1.0                 ║\n";
    std::cout << "║       Modern C++17 File Organization Tool                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    while (isRunning) {
        try {
            displayMainMenu();
            int choice = getIntInput("\nEnter your choice: ");
            processChoice(choice);
            
        } catch (const std::exception& e) {
            /**
             * Teaching Point: CATCH-ALL EXCEPTION HANDLER
             * 
             * std::exception is base class for all standard exceptions:
             * - std::runtime_error
             * - std::logic_error
             * - std::filesystem::filesystem_error
             * - etc.
             * 
             * Catching std::exception catches ALL of these
             * This is a safety net for unexpected errors
             */
            std::cerr << "\n❌ Error: " << e.what() << "\n";
            Logger::getInstance().log("ERROR: " + std::string(e.what()));
            pauseScreen();
        }
    }
    
    std::cout << "\n👋 Thank you for using Smart File Management System!\n\n";
    Logger::getInstance().log("Application terminated normally");
}

/**
 * @brief Displays main menu
 * 
 * Teaching Point: USER INTERFACE DESIGN
 * 
 * Good menu design:
 * 1. Clear options with numbers
 * 2. Visual hierarchy (separators, symbols)
 * 3. Current state display (directory path)
 * 4. Consistent formatting
 * 5. Exit option clearly visible
 */
void Menu::displayMainMenu() const {
    clearScreen();
    
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📂 Current Directory: " << currentDirectory << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::cout << "  1️⃣  Scan Directory\n";
    std::cout << "  2️⃣  Organize Files by Extension\n";
    std::cout << "  3️⃣  Search Files by Name\n";
    std::cout << "  4️⃣  Find Duplicate Files\n";
    std::cout << "  5️⃣  Display All Files\n";
    std::cout << "  6️⃣  Change Directory\n";
    std::cout << "  7️⃣  View Category Mappings\n";
    std::cout << "  0️⃣  Exit\n\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

/**
 * @brief Routes user choice to appropriate handler
 * @param choice Menu option number
 * 
 * Teaching Point: SWITCH STATEMENT ROUTING
 * 
 * PATTERN: Command Routing
 * 
 * Each case calls a specific handler method
 * This is simple and efficient for small menus
 * 
 * ALTERNATIVE: Command Pattern
 * std::map<int, std::function<void()>> commands;
 * commands[1] = [this]() { handleScanDirectory(); };
 * commands[choice]();
 * 
 * Command pattern is more flexible but overkill for this case
 */
void Menu::processChoice(int choice) {
    switch (choice) {
        case 1:
            handleScanDirectory();
            break;
        case 2:
            handleOrganizeFiles();
            break;
        case 3:
            handleSearchFiles();
            break;
        case 4:
            handleFindDuplicates();
            break;
        case 5:
            handleDisplayFiles();
            break;
        case 6:
            handleChangeDirectory();
            break;
        case 7:
            fileSorter->displayCategories();
            pauseScreen();
            break;
        case 0:
            exit();
            break;
        default:
            std::cout << "\n❌ Invalid choice! Please enter 0-7.\n";
            pauseScreen();
    }
}

/**
 * @brief Handler: Scan directory for files
 * 
 * Teaching Point: HANDLER METHOD PATTERN
 * 
 * Each feature gets its own method
 * Benefits:
 * - Single Responsibility Principle
 * - Easy to test individually
 * - Easy to modify without affecting others
 * - Clear code organization
 */
void Menu::handleScanDirectory() {
    std::cout << "\n🔍 Scanning directory: " << currentDirectory << "\n\n";
    
    int count = fileManager->scanDirectory();
    
    if (count > 0) {
        std::cout << "✅ Found " << count << " files!\n";
    } else {
        std::cout << "⚠️  No files found or directory is empty.\n";
    }
    
    pauseScreen();
}

/**
 * @brief Handler: Organize files into categories
 * 
 * Teaching Point: USER CONFIRMATION PATTERN
 * 
 * For destructive operations (moving files), always:
 * 1. Show what will happen
 * 2. Ask for confirmation
 * 3. Provide clear success/failure feedback
 */
void Menu::handleOrganizeFiles() {
    const auto& files = fileManager->getFiles();
    
    if (files.empty()) {
        std::cout << "\n⚠️  No files scanned yet. Please scan directory first.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n📁 Ready to organize " << files.size() << " files.\n";
    std::cout << "Files will be moved into category-based subfolders.\n\n";
    
    std::string confirm = getUserInput("Proceed with organization? (yes/no): ");
    
    /**
     * Teaching Point: STRING COMPARISON
     * 
     * Direct comparison: if (confirm == "yes")
     * Works for exact match
     * 
     * For case-insensitive: convert to lowercase first
     * Accepts "yes", "YES", "Yes", etc.
     */
    std::transform(confirm.begin(), confirm.end(), confirm.begin(), ::tolower);
    
    if (confirm != "yes" && confirm != "y") {
        std::cout << "\n❌ Organization cancelled.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n🔄 Organizing files...\n\n";
    int movedCount = fileSorter->organizeByExtension(files, currentDirectory);
    
    std::cout << "\n✅ Organization complete! " << movedCount 
              << " files moved.\n";
    
    // Rescan to update file list after organization
    std::cout << "\n🔄 Rescanning directory...\n";
    fileManager->scanDirectory();
    
    pauseScreen();
}

/**
 * @brief Handler: Search files by name
 * 
 * Teaching Point: SEARCH INTERACTION PATTERN
 * 
 * 1. Get search term from user
 * 2. Validate input (not empty)
 * 3. Perform search
 * 4. Display results
 */
void Menu::handleSearchFiles() {
    const auto& files = fileManager->getFiles();
    
    if (files.empty()) {
        std::cout << "\n⚠️  No files scanned yet. Please scan directory first.\n";
        pauseScreen();
        return;
    }
    
    std::string searchTerm = getUserInput("\nEnter filename to search: ");
    
    if (searchTerm.empty()) {
        std::cout << "\n❌ Search term cannot be empty.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n🔍 Searching for: " << searchTerm << "\n";
    
    auto results = fileSearcher->searchByName(files, searchTerm);
    fileSearcher->displaySearchResults(results);
    
    pauseScreen();
}

/**
 * @brief Handler: Find duplicate files
 * 
 * Teaching Point: RESULT HANDLING PATTERN
 * 
 * Check result state before displaying:
 * - Empty results: Show "none found"
 * - Results found: Display them
 */
void Menu::handleFindDuplicates() {
    const auto& files = fileManager->getFiles();
    
    if (files.empty()) {
        std::cout << "\n⚠️  No files scanned yet. Please scan directory first.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n🔍 Analyzing " << files.size() << " files for duplicates...\n";
    
    auto duplicates = fileSearcher->findDuplicates(files);
    fileSearcher->displayDuplicates(duplicates);
    
    pauseScreen();
}

/**
 * @brief Handler: Display all scanned files
 * 
 * Teaching Point: PAGINATION CONCEPT
 * 
 * For production code with many files, implement pagination:
 * - Show 20 files per page
 * - "Press any key for next page"
 * - Prevents overwhelming the user
 * 
 * Here we show all (educational simplicity)
 */
void Menu::handleDisplayFiles() {
    const auto& files = fileManager->getFiles();
    
    if (files.empty()) {
        std::cout << "\n⚠️  No files scanned yet. Please scan directory first.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         ALL FILES (" << files.size() << " total)";
    int padding = 59 - std::to_string(files.size()).length() - 17;
    std::cout << std::string(padding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << std::left
              << std::setw(40) << "Filename"
              << std::setw(15) << "Size"
              << std::setw(10) << "Extension" << "\n";
    std::cout << std::string(65, '-') << "\n";
    
    for (const auto& file : files) {
        std::string sizeStr = std::to_string(file.size) + " B";
        
        std::cout << std::left
                  << std::setw(40) << file.name
                  << std::setw(15) << sizeStr
                  << std::setw(10) << file.extension << "\n";
    }
    
    std::cout << "\n";
    pauseScreen();
}

/**
 * @brief Handler: Change working directory
 * 
 * Teaching Point: STATE MANAGEMENT
 * 
 * Changing directory requires:
 * 1. Get new path from user
 * 2. Validate path exists
 * 3. Update state (currentDirectory)
 * 4. Update dependencies (fileManager)
 * 5. Clear old data (rescan needed)
 */
void Menu::handleChangeDirectory() {
    std::string newDir = getUserInput("\nEnter new directory path: ");
    
    if (newDir.empty()) {
        std::cout << "\n❌ Directory path cannot be empty.\n";
        pauseScreen();
        return;
    }
    
    // Create temporary FileManager to test path
    /**
     * Teaching Point: VALIDATION BEFORE STATE CHANGE
     * 
     * Don't change state until validation succeeds
     * This maintains consistency (class invariant)
     */
    try {
        auto testManager = std::make_shared<FileManager>(newDir);
        if (!testManager->directoryExists()) {
            std::cout << "\n❌ Directory does not exist: " << newDir << "\n";
            pauseScreen();
            return;
        }
        
        // Validation passed, update state
        currentDirectory = newDir;
        fileManager = testManager;  // Replace with validated manager
        
        std::cout << "\n✅ Directory changed successfully!\n";
        std::cout << "📂 New directory: " << currentDirectory << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Error changing directory: " << e.what() << "\n";
    }
    
    pauseScreen();
}

/**
 * @brief Exits the application
 * 
 * Teaching Point: GRACEFUL SHUTDOWN
 * 
 * Proper exit sequence:
 * 1. Set running flag to false
 * 2. Log shutdown event
 * 3. Let destructors clean up (RAII)
 */
void Menu::exit() {
    isRunning = false;
    Logger::getInstance().log("User requested exit");
}

/**
 * @brief Gets string input from user
 * @param prompt Prompt message
 * @return User input
 * 
 * Teaching Point: INPUT VALIDATION
 * 
 * Always validate user input:
 * - Check for empty input
 * - Trim whitespace
 * - Validate format
 * 
 * Here we do basic input, caller does validation
 */
std::string Menu::getUserInput(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

/**
 * @brief Gets integer input with validation
 * @param prompt Prompt message
 * @return Valid integer
 * 
 * Teaching Point: ROBUST INTEGER INPUT
 * 
 * PROBLEM: cin >> int can fail on non-numeric input
 * Example: User types "abc" → cin fails, int has garbage value
 * 
 * SOLUTION:
 * 1. Read as string (always succeeds)
 * 2. Try to convert to int (std::stoi)
 * 3. Catch exception if conversion fails
 * 4. Loop until valid input received
 */
int Menu::getIntInput(const std::string& prompt) const {
    while (true) {
        std::string input = getUserInput(prompt);
        
        try {
            /**
             * Teaching Point: std::stoi (String TO Integer)
             * 
             * Converts string to int
             * Throws std::invalid_argument if not a number
             * Throws std::out_of_range if number too large
             * 
             * ALTERNATIVES:
             * - std::stol (string to long)
             * - std::stoll (string to long long)
             * - std::stod (string to double)
             */
            int value = std::stoi(input);
            return value;
            
        } catch (const std::invalid_argument&) {
            std::cout << "❌ Invalid input! Please enter a number: ";
        } catch (const std::out_of_range&) {
            std::cout << "❌ Number too large! Please enter a valid number: ";
        }
    }
}

/**
 * @brief Pauses screen and waits for user
 * 
 * Teaching Point: USER FLOW CONTROL
 * 
 * After displaying results, pause so user can read
 * Otherwise, menu immediately refreshes and user misses output
 */
void Menu::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

/**
 * @brief Clears console screen
 * 
 * Teaching Point: PLATFORM-SPECIFIC CODE
 * 
 * Screen clearing is platform-dependent:
 * - Windows: system("cls")
 * - Unix/Linux: system("clear")
 * 
 * Better approach: ANSI escape codes (more portable)
 * std::cout << "\033[2J\033[1;1H";
 * 
 * For production: Use library like ncurses
 */
void Menu::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM MENU IMPLEMENTATION
 * =============================================================================
 * 
 * 1. MVC CONTROLLER PATTERN:
 *    - Separates UI logic from business logic
 *    - Coordinates between models (FileManager, etc.) and view (console)
 *    - Single point of user interaction
 * 
 * 2. DEPENDENCY INJECTION:
 *    - Dependencies passed in constructor
 *    - Loose coupling
 *    - Testability and flexibility
 * 
 * 3. EVENT LOOP:
 *    - Main application loop pattern
 *    - Display → Input → Process → Repeat
 *    - Clean termination handling
 * 
 * 4. INPUT VALIDATION:
 *    - Robust integer input
 *    - String validation
 *    - Error recovery loops
 * 
 * 5. USER EXPERIENCE:
 *    - Clear menu design
 *    - Visual feedback (emojis, symbols)
 *    - Confirmation for destructive operations
 *    - Screen pausing for readability
 * 
 * 6. ERROR HANDLING:
 *    - Try-catch blocks
 *    - Graceful degradation
 *    - User-friendly error messages
 * 
 * 7. STATE MANAGEMENT:
 *    - Validation before state changes
 *    - Consistent state maintenance
 *    - Proper cleanup on exit
 * 
 * INDUSTRY BEST PRACTICES:
 * - Separation of concerns (UI vs logic)
 * - Defensive programming (validate everything)
 * - User-centered design (clear feedback)
 * - Exception safety (catch all errors)
 * - Code organization (one method per feature)
 * - RAII for cleanup (smart pointers, destructors)
 */
