#include "../include/FileSorter.h"
#include "../include/Logger.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

/**
 * =============================================================================
 * FILESORTER IMPLEMENTATION - MASTERING std::map AND FILE OPERATIONS
 * =============================================================================
 * 
 * This class demonstrates:
 * 1. std::map usage for key-value lookups
 * 2. File moving operations with std::filesystem
 * 3. Directory creation with error handling
 * 4. Batch processing patterns
 */

/**
 * @brief Constructor - Initializes extension categories
 * 
 * Teaching Point: Initialization in constructor ensures object
 * is ready to use immediately after creation (class invariant).
 */
FileSorter::FileSorter() {
    initializeCategories();
    Logger::getInstance().log("FileSorter initialized with category mappings");
}

/**
 * @brief Populates the extension-to-category mapping
 * 
 * Teaching Point: std::map INSERTION METHODS
 * 
 * THREE WAYS TO INSERT:
 * 1. map[key] = value;           // Easiest, but overwrites if exists
 * 2. map.insert({key, value});   // Returns pair<iterator, bool>
 * 3. map.emplace(key, value);    // Constructs in-place (most efficient)
 * 
 * We use method 1 here for clarity (teaching context).
 * In production, emplace() is preferred for efficiency.
 * 
 * DESIGN DECISION: Predefined categories
 * Alternative: Could load from config file (more flexible but complex)
 * Current approach: Hardcoded (simple, fast, sufficient for most cases)
 */
void FileSorter::initializeCategories() {
    // Documents
    extensionCategories[".txt"] = "Documents";
    extensionCategories[".pdf"] = "Documents";
    extensionCategories[".doc"] = "Documents";
    extensionCategories[".docx"] = "Documents";
    extensionCategories[".xlsx"] = "Documents";
    extensionCategories[".xls"] = "Documents";
    extensionCategories[".ppt"] = "Documents";
    extensionCategories[".pptx"] = "Documents";
    extensionCategories[".odt"] = "Documents";
    extensionCategories[".rtf"] = "Documents";
    
    // Images
    extensionCategories[".jpg"] = "Images";
    extensionCategories[".jpeg"] = "Images";
    extensionCategories[".png"] = "Images";
    extensionCategories[".gif"] = "Images";
    extensionCategories[".bmp"] = "Images";
    extensionCategories[".svg"] = "Images";
    extensionCategories[".ico"] = "Images";
    extensionCategories[".tiff"] = "Images";
    extensionCategories[".webp"] = "Images";
    
    // Videos
    extensionCategories[".mp4"] = "Videos";
    extensionCategories[".avi"] = "Videos";
    extensionCategories[".mkv"] = "Videos";
    extensionCategories[".mov"] = "Videos";
    extensionCategories[".wmv"] = "Videos";
    extensionCategories[".flv"] = "Videos";
    extensionCategories[".webm"] = "Videos";
    extensionCategories[".m4v"] = "Videos";
    
    // Audio
    extensionCategories[".mp3"] = "Audio";
    extensionCategories[".wav"] = "Audio";
    extensionCategories[".flac"] = "Audio";
    extensionCategories[".aac"] = "Audio";
    extensionCategories[".ogg"] = "Audio";
    extensionCategories[".wma"] = "Audio";
    extensionCategories[".m4a"] = "Audio";
    
    // Archives
    extensionCategories[".zip"] = "Archives";
    extensionCategories[".rar"] = "Archives";
    extensionCategories[".7z"] = "Archives";
    extensionCategories[".tar"] = "Archives";
    extensionCategories[".gz"] = "Archives";
    extensionCategories[".bz2"] = "Archives";
    extensionCategories[".xz"] = "Archives";
    
    // Code files
    extensionCategories[".cpp"] = "Code";
    extensionCategories[".h"] = "Code";
    extensionCategories[".hpp"] = "Code";
    extensionCategories[".c"] = "Code";
    extensionCategories[".py"] = "Code";
    extensionCategories[".java"] = "Code";
    extensionCategories[".js"] = "Code";
    extensionCategories[".ts"] = "Code";
    extensionCategories[".html"] = "Code";
    extensionCategories[".css"] = "Code";
    extensionCategories[".php"] = "Code";
    extensionCategories[".rb"] = "Code";
    extensionCategories[".go"] = "Code";
    extensionCategories[".rs"] = "Code";
    
    // Executables
    extensionCategories[".exe"] = "Executables";
    extensionCategories[".dll"] = "Executables";
    extensionCategories[".so"] = "Executables";
    extensionCategories[".app"] = "Executables";
    extensionCategories[".deb"] = "Executables";
    extensionCategories[".rpm"] = "Executables";
}

/**
 * @brief Looks up category for given extension
 * @param extension File extension (e.g., ".txt")
 * @return Category name or "Others" if not found
 * 
 * Teaching Point: SAFE MAP LOOKUP PATTERN
 * 
 * WRONG WAY:
 * return extensionCategories[extension];  // Creates entry if not found!
 * 
 * RIGHT WAY: Use find()
 * 
 * std::map::find() returns iterator:
 * - If found: iterator points to key-value pair
 * - If not found: iterator equals map.end()
 * 
 * This is the standard STL pattern for safe lookups.
 * 
 * ALTERNATIVE: C++20 contains()
 * if (extensionCategories.contains(extension)) { ... }
 * But find() works in C++17 and earlier.
 */
std::string FileSorter::getCategoryForExtension(const std::string& extension) const {
    auto it = extensionCategories.find(extension);
    
    /**
     * Teaching Point: AUTO KEYWORD
     * 
     * auto it = ...
     * 
     * Compiler deduces type automatically:
     * std::map<std::string, std::string>::const_iterator
     * 
     * Benefits:
     * - Less typing
     * - Easier to refactor (change map type, auto adapts)
     * - Cleaner code
     * 
     * When to use auto:
     * - Complex types (iterators, lambda functions)
     * - Obvious types from right-hand side
     * 
     * When NOT to use auto:
     * - When type clarity is important
     * - When implicit conversions matter
     */
    
    if (it != extensionCategories.end()) {
        return it->second;  // Return the value (category)
    }
    return "Others";  // Default category for unknown extensions
}

/**
 * @brief Creates directory if it doesn't exist
 * @param path Directory path
 * @return true if successful or already exists
 * 
 * Teaching Point: IDEMPOTENT OPERATIONS
 * 
 * Idempotent = Can be called multiple times with same result
 * 
 * This function:
 * - If directory exists: Returns true (no action needed)
 * - If doesn't exist: Creates it and returns true
 * - If error: Returns false
 * 
 * This is safer than assuming directory doesn't exist and
 * always trying to create (which would error if it exists).
 */
bool FileSorter::createDirectoryIfNotExists(const std::string& path) {
    try {
        if (fs::exists(path)) {
            return true;  // Already exists, nothing to do
        }
        
        /**
         * Teaching Point: fs::create_directories() vs fs::create_directory()
         * 
         * create_directory():  Creates ONE level (fails if parent doesn't exist)
         * create_directories(): Creates ALL levels (like mkdir -p)
         * 
         * Example: "/home/user/projects/new_folder/subfolder"
         * - create_directory(): FAILS if new_folder doesn't exist
         * - create_directories(): Creates new_folder AND subfolder
         * 
         * Always use create_directories() unless you specifically want to
         * fail if parent doesn't exist.
         */
        return fs::create_directories(path);
        
    } catch (const fs::filesystem_error& e) {
        Logger::getInstance().log("ERROR creating directory " + path + ": " + e.what());
        return false;
    }
}

/**
 * @brief Main organization algorithm
 * @param files Vector of files to organize
 * @param baseDirectory Root directory for organization
 * @return Number of files successfully moved
 * 
 * Teaching Point: BATCH PROCESSING PATTERN
 * 
 * ALGORITHM:
 * 1. For each file:
 *    a. Determine category from extension
 *    b. Create category folder (if needed)
 *    c. Construct destination path
 *    d. Move file
 *    e. Log operation
 * 2. Count successes
 * 3. Return count
 * 
 * ERROR HANDLING STRATEGY:
 * - Try-catch inside loop (graceful degradation)
 * - If one file fails, continue with others
 * - Log each failure for debugging
 * - Return count of successes
 * 
 * ALTERNATIVE STRATEGY (fail-fast):
 * - Try-catch outside loop
 * - First error stops entire operation
 * - All-or-nothing approach
 * 
 * Choice depends on use case. Here, partial success is acceptable.
 */
int FileSorter::organizeByExtension(const std::vector<FileInfo>& files, 
                                    const std::string& baseDirectory) {
    int movedCount = 0;
    
    Logger::getInstance().log("Starting file organization in: " + baseDirectory);
    
    /**
     * Teaching Point: RANGE-BASED FOR LOOP with const reference
     * 
     * for (const auto& file : files)
     * 
     * Breakdown:
     * - auto: Compiler deduces type (FileInfo)
     * - const: We're not modifying the file object
     * - &: Pass by reference (avoid copying)
     * 
     * WHY const&?
     * - FileInfo might be large (path, strings)
     * - Copying is expensive (time + memory)
     * - We only read data, no modifications
     * 
     * ALTERNATIVES:
     * for (FileInfo file : files)         // Copies each file! Slow!
     * for (auto& file : files)            // Non-const ref (for modifying)
     * for (const FileInfo& file : files)  // Explicit type (no auto)
     */
    for (const auto& file : files) {
        try {
            // Step 1: Determine category
            std::string category = getCategoryForExtension(file.extension);
            
            // Step 2: Create category folder
            std::string categoryPath = baseDirectory + "/" + category;
            if (!createDirectoryIfNotExists(categoryPath)) {
                Logger::getInstance().log("ERROR: Failed to create directory: " + categoryPath);
                continue;  // Skip this file, move to next
            }
            
            // Step 3: Construct destination path
            std::string destPath = categoryPath + "/" + file.name;
            
            /**
             * Teaching Point: HANDLING FILE NAME CONFLICTS
             * 
             * What if file already exists at destination?
             * 
             * OPTIONS:
             * 1. Overwrite (dangerous - data loss!)
             * 2. Skip (safe but incomplete)
             * 3. Rename (add suffix like _1, _2)
             * 4. Ask user (interactive)
             * 
             * Here we skip for safety. Production code might rename.
             */
            if (fs::exists(destPath)) {
                Logger::getInstance().log("SKIPPED: File already exists: " + destPath);
                continue;
            }
            
            // Step 4: Move file
            /**
             * Teaching Point: fs::rename() for moving files
             * 
             * Despite the name, rename() MOVES files:
             * - Same filesystem: Instant (just updates directory entry)
             * - Different filesystem: Copies then deletes (slower)
             * 
             * ALTERNATIVE: fs::copy() + fs::remove()
             * - More explicit
             * - Allows copy without removing original
             * - But rename() is atomic (safer)
             */
            fs::rename(file.path, destPath);
            
            // Step 5: Log success
            Logger::getInstance().log("Moved: " + file.name + " → " + category + "/");
            movedCount++;
            
        } catch (const fs::filesystem_error& e) {
            /**
             * Teaching Point: PER-FILE ERROR HANDLING
             * 
             * Catch inside loop = graceful degradation
             * One file failure doesn't stop the entire operation
             * 
             * Log error for debugging, then continue
             */
            std::string errorMsg = "ERROR moving " + file.name + ": " + e.what();
            Logger::getInstance().log(errorMsg);
            std::cerr << errorMsg << std::endl;
        }
    }
    
    Logger::getInstance().log("Organization complete: " + 
                            std::to_string(movedCount) + " files moved");
    return movedCount;
}

/**
 * @brief Displays all category mappings
 * 
 * Teaching Point: ITERATING THROUGH std::map
 * 
 * for (const auto& pair : map)
 * 
 * Each element in map is std::pair<Key, Value>
 * - pair.first = key (extension)
 * - pair.second = value (category)
 * 
 * C++17 STRUCTURED BINDINGS (cleaner alternative):
 * for (const auto& [ext, cat] : extensionCategories) {
 *     std::cout << ext << " → " << cat << "\n";
 * }
 * 
 * This is more readable than .first/.second!
 */
void FileSorter::displayCategories() const {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║     EXTENSION CATEGORY MAPPINGS        ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    // Group by category for better readability
    std::map<std::string, std::vector<std::string>> categoryGroups;
    
    /**
     * Teaching Point: REVERSE MAPPING
     * 
     * Original: extension → category
     * Goal: category → [list of extensions]
     * 
     * Algorithm:
     * 1. Iterate through extension map
     * 2. For each (ext, cat) pair:
     *    - Add ext to categoryGroups[cat] vector
     * 3. Result: Grouped view
     */
    for (const auto& [ext, cat] : extensionCategories) {
        categoryGroups[cat].push_back(ext);
    }
    
    // Display grouped
    for (const auto& [category, extensions] : categoryGroups) {
        std::cout << "📁 " << category << ": ";
        for (size_t i = 0; i < extensions.size(); ++i) {
            std::cout << extensions[i];
            if (i < extensions.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM FILESORTER IMPLEMENTATION
 * =============================================================================
 * 
 * 1. STD::MAP MASTERY:
 *    - Insertion with []
 *    - Safe lookup with find()
 *    - Iteration with range-based for
 *    - Key-value pair access (.first, .second)
 * 
 * 2. FILE OPERATIONS:
 *    - fs::rename() for moving files
 *    - fs::create_directories() for folder creation
 *    - fs::exists() for conflict detection
 * 
 * 3. ERROR HANDLING:
 *    - Per-item try-catch (graceful degradation)
 *    - Logging all errors
 *    - Continue on failure
 * 
 * 4. ALGORITHM DESIGN:
 *    - Batch processing pattern
 *    - Idempotent operations
 *    - Success counting
 * 
 * 5. MODERN C++ FEATURES:
 *    - auto keyword for type deduction
 *    - Range-based for loops
 *    - Structured bindings (C++17)
 *    - const reference parameters (efficiency)
 * 
 * 6. CODE ORGANIZATION:
 *    - Clear method names (self-documenting)
 *    - Separation of concerns (each method does one thing)
 *    - Helper methods for reusability
 * 
 * INDUSTRY PATTERNS:
 * - Defensive programming (check before action)
 * - Comprehensive logging (audit trail)
 * - Graceful degradation (partial success acceptable)
 * - Clean error messages (user-friendly)
 */
