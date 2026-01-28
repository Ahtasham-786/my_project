#include "../include/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * =============================================================================
 * LOGGER IMPLEMENTATION - SINGLETON PATTERN IN ACTION
 * =============================================================================
 * 
 * This file demonstrates several advanced C++ concepts:
 * 1. Singleton Pattern implementation
 * 2. Thread-safe file operations using std::mutex
 * 3. RAII (Resource Acquisition Is Initialization)
 * 4. Modern C++ chrono library for timestamps
 */

/**
 * @brief Private Constructor Implementation
 * 
 * Teaching Point: This constructor opens the log file ONCE when the
 * singleton instance is created. The file remains open for the entire
 * program lifetime (efficiency - avoid open/close overhead per log).
 * 
 * std::ios::app = append mode
 * - New logs added to end
 * - Previous logs preserved
 * - Creates file if doesn't exist
 */
Logger::Logger() {
    logFile.open("file_manager.log", std::ios::app);
    if (!logFile.is_open()) {
        // Can't use log() here (recursive!), so use cerr
        std::cerr << "ERROR: Failed to open log file!" << std::endl;
    } else {
        // Log the logger's initialization (meta-logging!)
        log("=== File Management System Started ===");
    }
}

/**
 * @brief Destructor - RAII cleanup
 * 
 * Teaching Point: Destructor automatically called when program exits.
 * Closes the log file gracefully, ensuring all buffered data is written.
 * 
 * RAII ensures this happens even if:
 * - Program crashes (stack unwinding)
 * - Exception thrown
 * - Early return/exit
 */
Logger::~Logger() {
    if (logFile.is_open()) {
        log("=== File Management System Stopped ===");
        logFile.close();
    }
}

/**
 * @brief Static getInstance() - Heart of Singleton Pattern
 * 
 * Teaching Point: "Magic Statics" (C++11)
 * 
 * static Logger instance;
 * 
 * This line creates the instance on FIRST CALL only.
 * Subsequent calls return the same instance.
 * 
 * THREAD SAFETY: C++11 guarantees thread-safe initialization of
 * function-local static variables. Multiple threads calling this
 * simultaneously will wait until initialization completes.
 * 
 * Why reference return?
 * - Prevents copying: Can't do Logger copy = getInstance();
 * - Enforces single instance: Always working with THE instance
 * - Efficient: No copying overhead
 */
Logger& Logger::getInstance() {
    static Logger instance;  // Created once, lives until program exit
    return instance;
}

/**
 * @brief Main logging method
 * @param message Text to log
 * 
 * Teaching Point: Thread-Safe Logging with std::lock_guard
 * 
 * SCENARIO WITHOUT MUTEX:
 * Thread 1: Writes "Action A"
 * Thread 2: Writes "Action B" (simultaneously)
 * Result: "ActAciotino Bn A" (interleaved/corrupted!)
 * 
 * SOLUTION: std::lock_guard
 * - Locks mutex on construction
 * - Unlocks on destruction (RAII)
 * - Automatic even with exceptions
 * 
 * HOW IT WORKS:
 * 1. Thread 1 enters, acquires lock
 * 2. Thread 2 tries to enter, WAITS (blocked)
 * 3. Thread 1 finishes, lock_guard destroyed, lock released
 * 4. Thread 2 now acquires lock and proceeds
 * 
 * Result: Serialized access, no corruption
 */
void Logger::log(const std::string& message) {
    // RAII lock - automatically releases when function exits
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        // Format: [2026-01-28 14:30:45] User action performed
        logFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
        
        // std::endl does two things:
        // 1. Inserts newline (\n)
        // 2. Flushes buffer (ensures immediate write to disk)
        // 
        // Why flush? Logs should be immediately visible, especially for debugging
    }
}

/**
 * @brief Generates timestamp for log entries
 * @return Formatted string: "YYYY-MM-DD HH:MM:SS"
 * 
 * Teaching Point: Modern C++ time handling with <chrono>
 * 
 * OLD WAY (C-style):
 * time_t now = time(NULL);
 * struct tm* timeinfo = localtime(&now);
 * 
 * NEW WAY (C++11 chrono):
 * - Type-safe (duration types: seconds, milliseconds, etc.)
 * - Portable across platforms
 * - More precise (nanosecond resolution available)
 * - Better integration with C++ STL
 */
std::string Logger::getCurrentTimestamp() const {
    // Get current time point
    auto now = std::chrono::system_clock::now();
    
    // Convert to time_t for formatting (C++ chrono doesn't have great formatting yet)
    // Note: C++20 adds std::format for better time formatting
    auto time = std::chrono::system_clock::to_time_t(now);
    
    // Create string stream for formatting
    std::stringstream ss;
    
    // std::put_time: Formats time according to format string
    // %Y = 4-digit year, %m = month, %d = day
    // %H = hour (24h), %M = minute, %S = second
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    
    return ss.str();
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM LOGGER IMPLEMENTATION
 * =============================================================================
 * 
 * 1. SINGLETON PATTERN:
 *    - Private constructor prevents external instantiation
 *    - Static getInstance() provides controlled access
 *    - Delete copy/move to prevent duplication
 * 
 * 2. THREAD SAFETY:
 *    - std::mutex prevents race conditions
 *    - std::lock_guard provides RAII-based locking
 *    - Magic statics ensure thread-safe initialization
 * 
 * 3. RAII PRINCIPLE:
 *    - File opened in constructor, closed in destructor
 *    - Lock acquired by lock_guard, released automatically
 *    - Exception-safe resource management
 * 
 * 4. MODERN C++:
 *    - <chrono> for time handling
 *    - <filesystem> compatibility (used by other classes)
 *    - Smart pointers readiness (this class works with unique_ptr/shared_ptr)
 * 
 * 5. BEST PRACTICES:
 *    - Const correctness (getCurrentTimestamp is const)
 *    - Error handling (check if file is open)
 *    - Clear method names (self-documenting code)
 *    - Single Responsibility (Logger only logs, nothing else)
 */
