#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>

/**
 * @brief Logger Class - Singleton Pattern Implementation
 * 
 * DESIGN PATTERN: Singleton
 * WHY? We need exactly ONE log file throughout the application's lifetime.
 * Multiple Logger instances could lead to:
 * - File corruption from concurrent writes
 * - Inconsistent log ordering
 * - Resource wastage
 * 
 * KEY CONCEPTS:
 * 1. Private constructor - prevents external instantiation
 * 2. Static instance method - controlled access point
 * 3. Thread-safe implementation using mutex
 * 4. Delete copy/move constructors - prevent duplication
 */
class Logger {
private:
    std::ofstream logFile;              // Output file stream for writing logs
    std::mutex logMutex;                // Thread-safety mechanism (prevents race conditions)
    
    /**
     * @brief Private Constructor - Core of Singleton Pattern
     * 
     * Teaching Point: By making constructor private, we prevent code like:
     * Logger myLogger;  // COMPILATION ERROR!
     * 
     * Opens log file in append mode (std::ios::app) so previous logs aren't lost.
     */
    Logger();
    
    /**
     * @brief Private Destructor - Ensures proper cleanup
     * 
     * Automatically closes the log file when program exits.
     * Teaching Point: RAII (Resource Acquisition Is Initialization) principle -
     * resources are tied to object lifetime.
     */
    ~Logger();

public:
    /**
     * @brief Delete copy constructor - Prevents copying
     * 
     * Teaching Point: = delete is C++11 syntax that explicitly disables a function.
     * Prevents: Logger copy = Logger::getInstance(); // ERROR!
     */
    Logger(const Logger&) = delete;
    
    /**
     * @brief Delete copy assignment - Prevents assignment
     * 
     * Prevents: Logger log1 = Logger::getInstance(); // ERROR!
     */
    Logger& operator=(const Logger&) = delete;
    
    /**
     * @brief Static method to get the single instance
     * @return Reference to the only Logger instance
     * 
     * Teaching Point: This is the ONLY way to access the Logger.
     * Uses "lazy initialization" - instance created on first call.
     * Thread-safe in C++11+ (magic statics guarantee).
     */
    static Logger& getInstance();
    
    /**
     * @brief Logs a message with timestamp
     * @param message The text to log
     * 
     * Teaching Point: Mutex ensures thread-safety. If multiple threads call this,
     * they'll wait in line (serialized access) rather than corrupting the file.
     * 
     * Format: [YYYY-MM-DD HH:MM:SS] Message
     */
    void log(const std::string& message);
    
    /**
     * @brief Helper to get current timestamp
     * @return Formatted timestamp string
     * 
     * Teaching Point: Private helper methods improve code organization.
     * This follows the Single Responsibility Principle - each method does one thing.
     */
    std::string getCurrentTimestamp() const;
};

#endif // LOGGER_H
