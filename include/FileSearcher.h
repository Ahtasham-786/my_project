#ifndef FILESEARCHER_H
#define FILESEARCHER_H

#include <vector>
#include <string>
#include <map>
#include "FileInfo.h"

/**
 * @brief FileSearcher Class - Advanced Search & Duplicate Detection
 * 
 * RESPONSIBILITIES:
 * 1. Partial filename matching (case-insensitive fuzzy search)
 * 2. Duplicate file detection using multiple strategies
 * 
 * ALGORITHMS IMPLEMENTED:
 * 1. Boyer-Moore-inspired substring search (for name matching)
 * 2. Hash-based duplicate detection (size + name hashing)
 * 3. Multi-criteria comparison (size, name, content hash)
 * 
 * Teaching Point: This class showcases algorithm design and STL mastery.
 * Different problems require different algorithms - choosing wisely
 * impacts performance dramatically.
 */
class FileSearcher {
private:
    /**
     * @brief Converts string to lowercase for case-insensitive comparison
     * @param str Input string
     * @return Lowercase version
     * 
     * Teaching Point: std::transform is an STL algorithm that applies
     * a function to each element. This is functional programming style in C++.
     * 
     * Usage: transform(str.begin(), str.end(), str.begin(), ::tolower);
     */
    std::string toLowercase(const std::string& str) const;
    
    /**
     * @brief Generates simple hash for duplicate detection
     * @param file FileInfo object
     * @return Hash string (combination of size and name)
     * 
     * Teaching Point: Simple hash = size + name
     * For production: use MD5/SHA-256 with actual file content.
     * Trade-off: speed vs accuracy
     * 
     * Simple hash: Fast, may have false positives (same size+name, different content)
     * Content hash: Slower, no false positives
     */
    std::string generateSimpleHash(const FileInfo& file) const;

public:
    /**
     * @brief Default constructor
     */
    FileSearcher() = default;
    
    /**
     * @brief Searches files by partial name match (case-insensitive)
     * @param files Vector of all files
     * @param searchTerm Partial filename to search for
     * @return Vector of matching files
     * 
     * Teaching Point: This demonstrates:
     * - STL algorithms (std::find_if with lambda)
     * - String manipulation
     * - Case-insensitive comparison
     * 
     * ALGORITHM:
     * 1. Convert search term to lowercase
     * 2. For each file:
     *    a. Convert filename to lowercase
     *    b. Check if search term is substring (str.find() != npos)
     *    c. If match, add to results vector
     * 3. Return results
     * 
     * TIME COMPLEXITY: O(n * m) where n = number of files, m = string length
     * 
     * Example:
     * Files: ["report.txt", "Report_2024.pdf", "summary.doc"]
     * Search: "report"
     * Results: ["report.txt", "Report_2024.pdf"] (case-insensitive)
     */
    std::vector<FileInfo> searchByName(const std::vector<FileInfo>& files, 
                                       const std::string& searchTerm) const;
    
    /**
     * @brief Finds duplicate files based on size and name
     * @param files Vector of all files
     * @return Map of hash to vector of duplicate files
     * 
     * Teaching Point: This uses std::map to group duplicates.
     * Key = hash (size + name)
     * Value = vector of files with that hash
     * 
     * DATA STRUCTURE CHOICE:
     * std::map<std::string, std::vector<FileInfo>>
     * 
     * WHY? We need to:
     * 1. Group files by hash (key)
     * 2. Store multiple files per hash (vector value)
     * 3. Efficiently check if hash exists (map.find())
     * 
     * ALGORITHM:
     * 1. Create empty map
     * 2. For each file:
     *    a. Generate hash (size + name)
     *    b. Add file to map[hash] vector
     * 3. Filter map: only return entries with 2+ files (actual duplicates)
     * 
     * TIME COMPLEXITY: O(n log n) for map insertions
     * SPACE COMPLEXITY: O(n) to store all files
     * 
     * Example Output:
     * {
     *   "1024_file.txt" → [file1.txt, file_copy.txt],
     *   "2048_image.jpg" → [pic1.jpg, pic2.jpg, pic3.jpg]
     * }
     */
    std::map<std::string, std::vector<FileInfo>> findDuplicates(
        const std::vector<FileInfo>& files) const;
    
    /**
     * @brief Displays search results in formatted table
     * @param results Vector of matching files
     * 
     * Teaching Point: Separation of concerns - display logic separated
     * from search logic. This follows Single Responsibility Principle.
     * 
     * Format:
     * +--------------------------------------------------+
     * | Name           | Size        | Extension | Path |
     * +--------------------------------------------------+
     * | report.txt     | 1024 bytes  | .txt      | ...  |
     * +--------------------------------------------------+
     */
    void displaySearchResults(const std::vector<FileInfo>& results) const;
    
    /**
     * @brief Displays duplicate files in grouped format
     * @param duplicates Map of duplicates
     * 
     * Teaching Point: Iterating through std::map
     * - map.begin() to map.end()
     * - Each element is std::pair<key, value>
     * - Access: it->first (key), it->second (value)
     * 
     * Format:
     * Duplicate Group 1:
     *   - file1.txt (1024 bytes)
     *   - file_copy.txt (1024 bytes)
     * 
     * Duplicate Group 2:
     *   - image1.jpg (2048 bytes)
     *   - image2.jpg (2048 bytes)
     */
    void displayDuplicates(
        const std::map<std::string, std::vector<FileInfo>>& duplicates) const;
};

#endif // FILESEARCHER_H
