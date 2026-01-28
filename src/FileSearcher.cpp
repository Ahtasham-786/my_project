#include "../include/FileSearcher.h"
#include "../include/Logger.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * =============================================================================
 * FILESEARCHER IMPLEMENTATION - ADVANCED ALGORITHMS & STL MASTERY
 * =============================================================================
 * 
 * This class demonstrates:
 * 1. String manipulation algorithms
 * 2. Hash-based duplicate detection
 * 3. STL algorithms (find_if, transform, etc.)
 * 4. Map-based grouping techniques
 * 5. Custom comparison logic
 */

/**
 * @brief Converts string to lowercase
 * @param str Input string
 * @return Lowercase version
 * 
 * Teaching Point: STL TRANSFORM ALGORITHM
 * 
 * std::transform signature:
 * transform(InputBegin, InputEnd, OutputBegin, UnaryFunction)
 * 
 * Applies UnaryFunction to each element in [Begin, End)
 * Stores results starting at OutputBegin
 * 
 * IN-PLACE TRANSFORMATION:
 * When InputBegin == OutputBegin, modifies in place
 * 
 * ::tolower - Global namespace tolower function
 * Why ::? Distinguishes from std::tolower (locale-aware version)
 */
std::string FileSearcher::toLowercase(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    /**
     * Teaching Point: LAMBDA FUNCTION
     * 
     * [](unsigned char c) { return std::tolower(c); }
     * 
     * Breakdown:
     * []                - Capture list (empty = captures nothing)
     * (unsigned char c) - Parameter
     * { ... }          - Function body
     * 
     * WHY unsigned char?
     * std::tolower expects value 0-255 or EOF
     * char can be negative on some systems (undefined behavior!)
     * unsigned char is always 0-255 (safe)
     * 
     * ALTERNATIVE:
     * Could use ::tolower directly, but lambda provides type safety
     */
    return result;
}

/**
 * @brief Generates simple hash for duplicate detection
 * @param file FileInfo object
 * @return Hash string
 * 
 * Teaching Point: HASHING STRATEGIES
 * 
 * SIMPLE HASH (used here):
 * - Combines size + name
 * - Fast (no file reading)
 * - May have false positives (same size+name, different content)
 * 
 * PRODUCTION HASH (recommended):
 * - Read file content
 * - Compute MD5/SHA-256
 * - Slower but accurate
 * - No false positives
 * 
 * TRADE-OFF:
 * Speed vs Accuracy
 * 
 * For this educational project, simple hash is sufficient.
 * In production file deduplication (like Dropbox), use content hash.
 * 
 * IMPLEMENTATION NOTE:
 * std::to_string() converts numbers to strings
 * + operator concatenates strings
 */
std::string FileSearcher::generateSimpleHash(const FileInfo& file) const {
    return std::to_string(file.size) + "_" + file.name;
}

/**
 * @brief Searches files by partial name match
 * @param files All files to search
 * @param searchTerm Partial name to find
 * @return Vector of matching files
 * 
 * Teaching Point: SUBSTRING SEARCH ALGORITHM
 * 
 * ALGORITHM:
 * 1. Convert search term to lowercase
 * 2. For each file:
 *    a. Convert filename to lowercase
 *    b. Check if search term is substring
 *    c. If yes, add to results
 * 3. Return results
 * 
 * TIME COMPLEXITY: O(n * m)
 * n = number of files
 * m = average string length
 * 
 * STRING::FIND() METHOD:
 * - Returns position of first occurrence
 * - Returns std::string::npos if not found
 * - Case-sensitive (that's why we convert to lowercase first)
 * 
 * EXAMPLE:
 * Files: ["report.txt", "Report_2024.pdf", "summary.doc"]
 * Search: "report"
 * 
 * After lowercase:
 * ["report.txt", "report_2024.pdf", "summary.doc"]
 * 
 * Matches: "report" found in first two
 * Results: ["report.txt", "Report_2024.pdf"]
 */
std::vector<FileInfo> FileSearcher::searchByName(
    const std::vector<FileInfo>& files, 
    const std::string& searchTerm) const {
    
    std::vector<FileInfo> results;
    std::string lowerSearchTerm = toLowercase(searchTerm);
    
    Logger::getInstance().log("Searching for files containing: " + searchTerm);
    
    /**
     * Teaching Point: ALTERNATIVE IMPLEMENTATION with STL ALGORITHM
     * 
     * Could use std::copy_if for functional programming style:
     * 
     * std::copy_if(files.begin(), files.end(), 
     *              std::back_inserter(results),
     *              [&](const FileInfo& file) {
     *                  std::string lowerName = toLowercase(file.name);
     *                  return lowerName.find(lowerSearchTerm) != std::string::npos;
     *              });
     * 
     * This is more concise but potentially less readable for beginners.
     * Explicit loop below is clearer for learning purposes.
     */
    
    for (const auto& file : files) {
        std::string lowerFileName = toLowercase(file.name);
        
        // Check if search term is substring of filename
        if (lowerFileName.find(lowerSearchTerm) != std::string::npos) {
            results.push_back(file);
            Logger::getInstance().log("Match found: " + file.name);
        }
    }
    
    Logger::getInstance().log("Search complete: " + 
                            std::to_string(results.size()) + " matches found");
    return results;
}

/**
 * @brief Finds duplicate files
 * @param files All files to check
 * @return Map of hash → vector of duplicate files
 * 
 * Teaching Point: HASH-BASED GROUPING ALGORITHM
 * 
 * PROBLEM: Find all files that are duplicates of each other
 * 
 * NAIVE APPROACH: Compare every file with every other file
 * - Time complexity: O(n²)
 * - For 1000 files: 1,000,000 comparisons!
 * 
 * HASH-BASED APPROACH: Group by hash
 * - Time complexity: O(n)
 * - For 1000 files: 1,000 comparisons!
 * 
 * ALGORITHM:
 * 1. Create empty map<hash, vector<files>>
 * 2. For each file:
 *    a. Generate hash
 *    b. Add file to map[hash] vector
 * 3. Filter: Only keep groups with 2+ files
 * 4. Return duplicate groups
 * 
 * DATA STRUCTURE: std::map<std::string, std::vector<FileInfo>>
 * 
 * Key = hash (identifies duplicate group)
 * Value = vector of files with that hash
 * 
 * EXAMPLE:
 * Files:
 * - file1.txt (1024 bytes)
 * - file2.txt (2048 bytes)
 * - file1_copy.txt (1024 bytes, same name)
 * - file3.txt (1024 bytes, different name)
 * 
 * After grouping:
 * {
 *   "1024_file1.txt" → [file1.txt, file1_copy.txt],
 *   "2048_file2.txt" → [file2.txt],
 *   "1024_file3.txt" → [file3.txt]
 * }
 * 
 * After filtering (2+ files only):
 * {
 *   "1024_file1.txt" → [file1.txt, file1_copy.txt]
 * }
 */
std::map<std::string, std::vector<FileInfo>> FileSearcher::findDuplicates(
    const std::vector<FileInfo>& files) const {
    
    // Step 1: Group by hash
    std::map<std::string, std::vector<FileInfo>> hashGroups;
    
    Logger::getInstance().log("Starting duplicate detection on " + 
                            std::to_string(files.size()) + " files");
    
    /**
     * Teaching Point: MAP INSERTION AND VECTOR PUSH_BACK
     * 
     * map[key] - If key doesn't exist, creates empty vector
     * .push_back(value) - Adds to that vector
     * 
     * This is idempotent - works whether key exists or not
     * 
     * FIRST INSERT:
     * hashGroups["hash1"] → creates empty vector
     * .push_back(file) → vector now has 1 element
     * 
     * SUBSEQUENT INSERTS:
     * hashGroups["hash1"] → returns existing vector
     * .push_back(file) → vector now has 2+ elements
     */
    for (const auto& file : files) {
        std::string hash = generateSimpleHash(file);
        hashGroups[hash].push_back(file);
    }
    
    // Step 2: Filter groups (only keep those with 2+ files)
    std::map<std::string, std::vector<FileInfo>> duplicates;
    
    /**
     * Teaching Point: FILTERING WITH CONDITIONAL INSERTION
     * 
     * We iterate through hashGroups and only copy groups with size >= 2
     * 
     * This is a filter operation - keeping only elements that meet criteria
     * 
     * ALTERNATIVE with STL:
     * Could use std::copy_if, but map filtering is less straightforward
     * Explicit loop is clearer here
     */
    for (const auto& [hash, group] : hashGroups) {
        if (group.size() >= 2) {
            duplicates[hash] = group;
            Logger::getInstance().log("Duplicate group found: " + 
                                    std::to_string(group.size()) + " files");
        }
    }
    
    Logger::getInstance().log("Duplicate detection complete: " + 
                            std::to_string(duplicates.size()) + " groups found");
    return duplicates;
}

/**
 * @brief Displays search results in formatted table
 * @param results Matching files
 * 
 * Teaching Point: FORMATTED OUTPUT with iomanip
 * 
 * <iomanip> provides stream manipulators:
 * - std::setw(width): Sets field width
 * - std::left/right: Alignment
 * - std::setfill(char): Fill character
 * - std::fixed/scientific: Number format
 * 
 * These affect the NEXT output operation
 * 
 * EXAMPLE:
 * std::cout << std::setw(20) << std::left << "Name";
 * 
 * Sets width to 20, left-aligned, then prints "Name"
 * If "Name" is 4 chars, adds 16 spaces after
 */
void FileSearcher::displaySearchResults(const std::vector<FileInfo>& results) const {
    if (results.empty()) {
        std::cout << "\n❌ No files found matching your search.\n\n";
        return;
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              SEARCH RESULTS (" << results.size() << " files)";
    // Dynamic padding to center
    int padding = 57 - std::to_string(results.size()).length() - 23;
    std::cout << std::string(padding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // Table header
    std::cout << std::left 
              << std::setw(40) << "Filename" 
              << std::setw(15) << "Size"
              << std::setw(10) << "Extension" << "\n";
    std::cout << std::string(65, '-') << "\n";
    
    /**
     * Teaching Point: NUMBER FORMATTING
     * 
     * For file sizes, we want human-readable format:
     * - 1024 bytes → "1.0 KB"
     * - 1048576 bytes → "1.0 MB"
     * 
     * ALGORITHM:
     * if (size < 1024) → bytes
     * else if (size < 1024*1024) → KB
     * else if (size < 1024*1024*1024) → MB
     * else → GB
     */
    for (const auto& file : results) {
        std::string sizeStr;
        double size = static_cast<double>(file.size);
        
        if (size < 1024) {
            sizeStr = std::to_string(static_cast<int>(size)) + " B";
        } else if (size < 1024 * 1024) {
            sizeStr = std::to_string(static_cast<int>(size / 1024)) + " KB";
        } else if (size < 1024 * 1024 * 1024) {
            sizeStr = std::to_string(static_cast<int>(size / (1024 * 1024))) + " MB";
        } else {
            sizeStr = std::to_string(static_cast<int>(size / (1024 * 1024 * 1024))) + " GB";
        }
        
        std::cout << std::left
                  << std::setw(40) << file.name
                  << std::setw(15) << sizeStr
                  << std::setw(10) << file.extension << "\n";
    }
    std::cout << "\n";
}

/**
 * @brief Displays duplicate groups
 * @param duplicates Map of duplicate groups
 * 
 * Teaching Point: NESTED ITERATION
 * 
 * We have map<string, vector<FileInfo>>
 * 
 * Outer loop: Iterate through map (each group)
 * Inner loop: Iterate through vector (files in group)
 * 
 * This is a common pattern for hierarchical data
 */
void FileSearcher::displayDuplicates(
    const std::map<std::string, std::vector<FileInfo>>& duplicates) const {
    
    if (duplicates.empty()) {
        std::cout << "\n✅ No duplicate files found!\n\n";
        return;
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         DUPLICATE FILES (" << duplicates.size() << " groups)";
    int padding = 58 - std::to_string(duplicates.size()).length() - 25;
    std::cout << std::string(padding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    int groupNum = 1;
    
    /**
     * Teaching Point: STRUCTURED BINDINGS (C++17)
     * 
     * for (const auto& [hash, files] : duplicates)
     * 
     * Unpacks std::pair<const string, vector> into:
     * - hash (first element)
     * - files (second element)
     * 
     * BEFORE C++17:
     * for (const auto& pair : duplicates) {
     *     const auto& hash = pair.first;
     *     const auto& files = pair.second;
     * }
     * 
     * C++17 version is much cleaner!
     */
    for (const auto& [hash, files] : duplicates) {
        std::cout << "📦 Duplicate Group #" << groupNum++ << " (" 
                  << files.size() << " files):\n";
        std::cout << std::string(60, '-') << "\n";
        
        for (const auto& file : files) {
            std::cout << "  📄 " << file.name << " (" << file.size << " bytes)\n";
            std::cout << "     Path: " << file.path.string() << "\n\n";
        }
    }
}

/**
 * =============================================================================
 * KEY TAKEAWAYS FROM FILESEARCHER IMPLEMENTATION
 * =============================================================================
 * 
 * 1. STL ALGORITHMS:
 *    - std::transform for string manipulation
 *    - Lambda functions for custom operations
 *    - Range-based for loops
 * 
 * 2. STRING OPERATIONS:
 *    - find() for substring search
 *    - std::string::npos for not found
 *    - tolower() for case-insensitive comparison
 * 
 * 3. HASH-BASED ALGORITHMS:
 *    - O(n) duplicate detection vs O(n²) naive approach
 *    - Map-based grouping
 *    - Trade-off: speed vs accuracy
 * 
 * 4. DATA STRUCTURES:
 *    - std::map for key-value storage
 *    - std::vector for dynamic arrays
 *    - Nested containers (map of vectors)
 * 
 * 5. FORMATTED OUTPUT:
 *    - <iomanip> manipulators
 *    - std::setw(), std::left
 *    - Human-readable file sizes
 * 
 * 6. MODERN C++ FEATURES:
 *    - Structured bindings (C++17)
 *    - Lambda expressions
 *    - auto keyword
 *    - Range-based for loops
 * 
 * 7. ALGORITHM OPTIMIZATION:
 *    - Avoid O(n²) when O(n) possible
 *    - Use appropriate data structures
 *    - Consider trade-offs (speed vs accuracy)
 * 
 * 8. CODE CLARITY:
 *    - Separate display logic from search logic
 *    - Helper methods for reusability
 *    - Clear variable names
 *    - Comprehensive comments
 * 
 * INDUSTRY SKILLS DEMONSTRATED:
 * - Algorithm design and complexity analysis
 * - Appropriate data structure selection
 * - Performance optimization techniques
 * - User-friendly output formatting
 * - Scalable code architecture
 */
