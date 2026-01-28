#include "../include/FileManager.h"
#include "../include/Logger.h"
#include <iostream>
#include <algorithm>

/**
 * =============================================================================
 * FILEMANAGER IMPLEMENTATION - MASTERING std::filesystem
 * =============================================================================
 * 
 * This class is your gateway to understanding modern C++ file operations.
 * Pre-C++17, file operations were platform-specific (Windows vs Unix).
 * C++17's <filesystem> provides cross-platform, type-safe file handling.
 * 
 * KEY CONCEPTS DEMONSTRATED:
 * 1. std::filesystem API usage
 * 2. STL container manipulation (std::vector)
 * 3. Exception handling with filesystem operations
 * 4. RAII with ofstream/ifstream (though not heavily used here)
 */

/**
 * @brief Constructor - Validates and stores target directory
 * @param dirPath Directory to manage
 * 
 * Teaching Point: "explicit" keyword in header
 * 
 * WITHOUT explicit:
 * FileManager fm = "/some/path";  // Implicit conversion! Dangerous!
 * 
 * WITH explicit:
 * FileManager fm = "/some/path";  // COMPILATION ERROR
 * FileManager fm("/some/path");   // OK - explicit construction
 * 
 * Why? Prevents accidental conversions that can hide bugs.
 */
FileManager::FileManager(const std::string& dirPath) 
    : targetDirectory(dirPath) {
    
    // Log initialization
    Logger::getInstance().log("FileManager initialized for directory: " + dirPath);
    
    // Teaching Point: Validation in constructor
    // Ensures object starts in valid state (class invariant)
    if (!directoryExists()) {
        Logger::getInstance().log("WARNING: Directory does not exist: " + dirPath);
        std::cerr << "Warning: Directory '" << dirPath << "' does not exist!\n";
    }
}

/**
 * @brief Validates directory existence
 * @return true if directory exists and is accessible
 * 
 * Teaching Point: std::filesystem::exists() and is_directory()
 * 
 * exists() checks if path exists (file OR directory)
 * is_directory() specifically checks if it's a directory
 * 
 * Why both? exists() alone could return true for a FILE!
 */
bool FileManager::directoryExists() const {
    try {
        fs::path dirPath(targetDirectory);
        return fs::exists(dirPath) && fs::is_directory(dirPath);
    } catch (const fs::filesystem_error& e) {
        // Possible errors:
        // - Permission denied
        // - Path too long
        // - Invalid characters
        Logger::getInstance().log("ERROR checking directory: " + std::string(e.what()));
        return false;
    }
}

/**
 * @brief Main scanning algorithm
 * @return Number of files found
 * 
 * Teaching Point: DIRECTORY ITERATION PATTERN
 * 
 * std::filesystem provides THREE iterators:
 * 1. directory_iterator - Current directory only (used here)
 * 2. recursive_directory_iterator - Includes subdirectories
 * 3. Both have range-based for loop support!
 * 
 * ALGORITHM BREAKDOWN:
 * 1. Clear previous scan results
 * 2. Iterate through directory entries
 * 3. Filter regular files (skip directories, symlinks)
 * 4. Extract metadata for each file
 * 5. Store in vector
 * 6. Return count
 */
int FileManager::scanDirectory() {
    files.clear();  // Clear previous scan (if any)
    
    if (!directoryExists()) {
        Logger::getInstance().log("ERROR: Cannot scan non-existent directory");
        return 0;
    }
    
    try {
        // Teaching Point: Range-based for loop with filesystem
        // Each entry is a directory_entry object containing:
        // - path (full path to file/directory)
        // - status (file type, permissions, etc.)
        // - Methods to query attributes
        
        for (const auto& entry : fs::directory_iterator(targetDirectory)) {
            // Filter: Only regular files (not directories or special files)
            if (entry.is_regular_file()) {
                FileInfo info = getFileInfo(entry.path());
                files.push_back(info);
                
                // Log each file found (verbose, but good for debugging)
                Logger::getInstance().log("Found file: " + info.name + 
                                        " (" + std::to_string(info.size) + " bytes)");
            }
        }
        
        Logger::getInstance().log("Scan complete: " + 
                                std::to_string(files.size()) + " files found");
        return static_cast<int>(files.size());
        
    } catch (const fs::filesystem_error& e) {
        /**
         * Teaching Point: Exception Handling Strategy
         * 
         * fs::filesystem_error inherits from std::system_error
         * Contains:
         * - what(): Error description
         * - path1(), path2(): Paths involved in operation
         * - code(): Error code (errno-like)
         * 
         * Common errors:
         * - Permission denied (can't read directory)
         * - Path not found
         * - Too many open files
         * - Disk I/O error
         */
        std::string errorMsg = "ERROR scanning directory: " + std::string(e.what());
        Logger::getInstance().log(errorMsg);
        std::cerr << errorMsg << std::endl;
        return 0;
    }
}

/**
 * @brief Extracts metadata from a single file
 * @param filePath Path to the file
 * @return FileInfo object with populated fields
 * 
 * Teaching Point: std::filesystem PATH OPERATIONS
 * 
 * fs::path has many useful methods:
 * - filename(): Returns filename with extension ("file.txt")
 * - extension(): Returns extension only (".txt")
 * - stem(): Returns filename without extension ("file")
 * - parent_path(): Returns directory containing the file
 * - is_absolute(), is_relative(): Path type checking
 */
FileInfo FileManager::getFileInfo(const fs::path& filePath) const {
    try {
        std::string filename = filePath.filename().string();
        std::string extension = extractExtension(filename);
        std::uintmax_t fileSize = fs::file_size(filePath);
        
        /**
         * Teaching Point: std::uintmax_t for file sizes
         * 
         * Why not int or long?
         * - int: Only 4 bytes on most systems (max 2GB files)
         * - long: Platform-dependent size
         * - uintmax_t: Guaranteed to hold largest possible file size
         * 
         * This ensures we can handle files larger than 4GB!
         */
        
        return FileInfo(filename, filePath, extension, fileSize);
        
    } catch (const fs::filesystem_error& e) {
        Logger::getInstance().log("ERROR reading file info: " + std::string(e.what()));
        // Return empty FileInfo on error (graceful degradation)
        return FileInfo();
    }
}

/**
 * @brief Extracts file extension from filename
 * @param filename Name of the file
 * @return Extension with dot (e.g., ".txt")
 * 
 * Teaching Point: STRING MANIPULATION ALGORITHM
 * 
 * APPROACH: Find last dot, extract substring
 * 
 * std::string::find_last_of() searches from right to left
 * std::string::npos = "not found" indicator
 * std::string::substr(pos) extracts from position to end
 * 
 * EXAMPLES:
 * "file.txt" → ".txt"
 * "archive.tar.gz" → ".gz" (only last extension)
 * "README" → "" (no extension)
 * ".gitignore" → ".gitignore" (entire name is extension)
 * 
 * ALTERNATIVE APPROACH:
 * Could use fs::path::extension(), but this teaches string manipulation!
 */
std::string FileManager::extractExtension(const std::string& filename) const {
    size_t dotPos = filename.find_last_of('.');
    
    // Teaching Point: npos check
    // std::string::npos is a special constant meaning "not found"
    // It's typically the maximum value of size_t
    if (dotPos != std::string::npos && dotPos > 0) {
        // Extract from dot to end
        std::string ext = filename.substr(dotPos);
        
        // Convert to lowercase for case-insensitive matching later
        // ".TXT" and ".txt" should be treated the same
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        /**
         * Teaching Point: std::transform with ::tolower
         * 
         * std::transform applies a function to each element
         * Signature: transform(begin, end, destination, function)
         * 
         * Here: in-place transformation (begin = destination)
         * ::tolower is C function (:: means global namespace)
         * 
         * Each character converted: 'T' → 't', 'X' → 'x', etc.
         */
        
        return ext;
    }
    
    return "";  // No extension found
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM FILEMANAGER IMPLEMENTATION
 * =============================================================================
 * 
 * 1. STD::FILESYSTEM USAGE:
 *    - directory_iterator for scanning
 *    - path methods for file information
 *    - is_regular_file(), exists(), is_directory() for filtering
 *    - file_size() for metadata
 * 
 * 2. EXCEPTION HANDLING:
 *    - Try-catch blocks around filesystem operations
 *    - fs::filesystem_error specific exceptions
 *    - Graceful degradation (return empty/default on error)
 * 
 * 3. STL CONTAINERS:
 *    - std::vector for dynamic file storage
 *    - push_back() for adding elements
 *    - clear() for resetting
 *    - size() for count
 * 
 * 4. STRING OPERATIONS:
 *    - find_last_of() for searching
 *    - substr() for extraction
 *    - transform() with tolower for normalization
 * 
 * 5. CONST CORRECTNESS:
 *    - Getter methods are const
 *    - Helper methods that don't modify state are const
 *    - Enables use with const objects
 * 
 * 6. LOGGING INTEGRATION:
 *    - Log all significant operations
 *    - Log errors for debugging
 *    - Singleton pattern usage (Logger::getInstance())
 * 
 * INDUSTRY BEST PRACTICES DEMONSTRATED:
 * - Validation in constructor (fail fast)
 * - Exception handling with specific catch blocks
 * - Const correctness throughout
 * - Clear separation of concerns (each method has one job)
 * - Comprehensive logging for audit trail
 * - Platform-independent code (std::filesystem is cross-platform)
 */
