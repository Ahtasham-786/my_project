#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <filesystem>
#include <string>
#include "FileInfo.h"

namespace fs = std::filesystem;

/**
 * @brief FileManager Class - Core File Operations Engine
 * 
 * RESPONSIBILITY: Central hub for all file system interactions
 * - Scanning directories
 * - Extracting file metadata
 * - Managing file collections
 * 
 * OOP PRINCIPLES DEMONSTRATED:
 * 1. Encapsulation: File operations hidden behind clean interface
 * 2. Single Responsibility: Only handles file system operations
 * 3. Abstraction: Hides complex <filesystem> API details
 * 
 * Teaching Point: This class acts as a "Facade" pattern, simplifying
 * the complex std::filesystem library for easier use.
 */
class FileManager {
private:
    std::vector<FileInfo> files;     // Container storing all scanned files
    std::string targetDirectory;     // Directory being managed
    
    /**
     * @brief Helper method to extract file extension
     * @param filename The filename to process
     * @return Extension with dot (e.g., ".txt")
     * 
     * Teaching Point: Private helper methods keep public interface clean.
     * This is called internally by getFileInfo().
     */
    std::string extractExtension(const std::string& filename) const;

public:
    /**
     * @brief Constructor with target directory
     * @param dirPath Path to directory to manage
     * 
     * Teaching Point: Constructor validates input and sets initial state.
     * Good practice: Validate in constructor to maintain class invariants.
     */
    explicit FileManager(const std::string& dirPath);
    
    /**
     * @brief Scans directory and populates files vector
     * @return Number of files found
     * @throws std::filesystem_error if directory doesn't exist or no permissions
     * 
     * Teaching Point: This method demonstrates:
     * - Exception handling (try-catch blocks)
     * - STL algorithms (vector operations)
     * - C++17 directory iteration
     * 
     * ALGORITHM:
     * 1. Clear existing files vector
     * 2. Iterate through directory entries
     * 3. Filter regular files (ignore directories/symlinks)
     * 4. Extract metadata for each file
     * 5. Store in files vector
     */
    int scanDirectory();
    
    /**
     * @brief Extracts metadata from a single file
     * @param filePath Path to the file
     * @return FileInfo object with populated metadata
     * 
     * Teaching Point: Separating concerns - this method focuses on ONE file,
     * making it reusable and testable independently.
     * 
     * Uses std::filesystem::file_size() for accurate size retrieval.
     */
    FileInfo getFileInfo(const fs::path& filePath) const;
    
    /**
     * @brief Getter for files collection
     * @return Const reference to files vector
     * 
     * Teaching Point: Returning const& prevents copying the entire vector
     * (efficiency) while preventing external modification (safety).
     * This is a best practice for getter methods.
     */
    const std::vector<FileInfo>& getFiles() const { return files; }
    
    /**
     * @brief Getter for target directory
     * @return Directory path
     */
    const std::string& getDirectory() const { return targetDirectory; }
    
    /**
     * @brief Checks if target directory exists
     * @return true if directory exists and is accessible
     * 
     * Teaching Point: Defensive programming - always validate before operations.
     */
    bool directoryExists() const;
};

#endif // FILEMANAGER_H
