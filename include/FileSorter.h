#ifndef FILESORTER_H
#define FILESORTER_H

#include <string>
#include <map>
#include <vector>
#include "FileInfo.h"

/**
 * @brief FileSorter Class - Smart File Organization Engine
 * 
 * RESPONSIBILITY: Categorizes and organizes files based on extensions
 * 
 * ALGORITHM: Extension-Based Categorization
 * - Maps extensions to logical categories (Documents, Images, Videos, etc.)
 * - Creates category folders if they don't exist
 * - Moves files to appropriate folders
 * - Logs all operations for audit trail
 * 
 * DATA STRUCTURE CHOICE: std::map
 * WHY? We need fast lookup: given extension → find category
 * std::map provides O(log n) lookup time with automatic sorting
 * 
 * Teaching Point: Choosing the right container is crucial for performance.
 * - std::vector: O(n) search, good for sequential access
 * - std::map: O(log n) search, good for key-based lookup
 * - std::unordered_map: O(1) average search, but no ordering
 */
class FileSorter {
private:
    /**
     * @brief Extension-to-Category mapping
     * 
     * Teaching Point: std::map stores key-value pairs in sorted order.
     * Key = extension (e.g., ".jpg")
     * Value = category name (e.g., "Images")
     * 
     * Example structure:
     * {
     *   ".txt" → "Documents",
     *   ".jpg" → "Images",
     *   ".mp4" → "Videos"
     * }
     */
    std::map<std::string, std::string> extensionCategories;
    
    /**
     * @brief Initializes the extension mapping
     * 
     * Teaching Point: Separation of concerns - initialization logic
     * separate from constructor for clarity and maintainability.
     * 
     * Populates extensionCategories with predefined mappings:
     * - Documents: .txt, .pdf, .doc, .docx, .xlsx, .ppt
     * - Images: .jpg, .jpeg, .png, .gif, .bmp, .svg
     * - Videos: .mp4, .avi, .mkv, .mov, .wmv
     * - Audio: .mp3, .wav, .flac, .aac
     * - Archives: .zip, .rar, .7z, .tar, .gz
     * - Code: .cpp, .h, .py, .java, .js, .html, .css
     */
    void initializeCategories();
    
    /**
     * @brief Creates a directory if it doesn't exist
     * @param path Directory path to create
     * @return true if created or already exists, false on error
     * 
     * Teaching Point: Error handling with boolean return.
     * Alternative: throw exception (more aggressive error handling)
     * Choice depends on whether failure is expected or exceptional.
     */
    bool createDirectoryIfNotExists(const std::string& path);

public:
    /**
     * @brief Constructor - initializes category mappings
     * 
     * Teaching Point: Constructor should set up invariants.
     * Here, we ensure the extension map is ready before any operations.
     */
    FileSorter();
    
    /**
     * @brief Organizes files into category-based subfolders
     * @param files Vector of files to organize
     * @param baseDirectory Root directory for organization
     * @return Number of files successfully moved
     * 
     * Teaching Point: This method demonstrates:
     * - Iterating through STL containers
     * - File system operations (create directories, move files)
     * - Error handling (try-catch for each file)
     * - Logging for audit trail
     * 
     * ALGORITHM:
     * 1. For each file in vector:
     *    a. Determine category from extension
     *    b. Create category folder if needed
     *    c. Construct destination path
     *    d. Move file using std::filesystem::rename()
     *    e. Log the operation
     * 2. Return count of successful moves
     * 
     * Teaching Point: Graceful degradation - if one file fails,
     * continue with others rather than stopping entirely.
     */
    int organizeByExtension(const std::vector<FileInfo>& files, 
                           const std::string& baseDirectory);
    
    /**
     * @brief Determines category for a given extension
     * @param extension File extension (e.g., ".txt")
     * @return Category name or "Others" if unknown
     * 
     * Teaching Point: std::map::find() returns iterator.
     * - If found: iterator points to key-value pair
     * - If not found: iterator equals map.end()
     * 
     * This is the standard STL pattern for safe lookups without exceptions.
     */
    std::string getCategoryForExtension(const std::string& extension) const;
    
    /**
     * @brief Displays all extension mappings (for user reference)
     * 
     * Teaching Point: Const method - doesn't modify object state.
     * Can be called on const objects. Good practice for read-only operations.
     */
    void displayCategories() const;
};

#endif // FILESORTER_H
