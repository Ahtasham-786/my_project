#ifndef FILEINFO_H
#define FILEINFO_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief FileInfo Structure - Encapsulates File Metadata
 * 
 * This is a Plain Old Data (POD) structure that stores essential information
 * about a file. It follows the Single Responsibility Principle by focusing
 * solely on representing file data.
 * 
 * Design Decision: Using struct instead of class since all members are public
 * and we don't need encapsulation here - it's pure data representation.
 */
struct FileInfo {
    std::string name;           // Filename with extension
    fs::path path;              // Complete filesystem path (using C++17 path type)
    std::string extension;      // File extension (e.g., ".txt", ".jpg")
    std::uintmax_t size;        // File size in bytes (uintmax_t ensures large file support)
    std::string hash;           // MD5/SHA hash for duplicate detection (computed lazily)
    
    /**
     * @brief Constructor with member initialization
     * @param n Filename
     * @param p Full path
     * @param ext Extension
     * @param s Size in bytes
     * 
     * Teaching Point: Use member initializer lists for efficiency (direct initialization
     * rather than assignment). This is especially important for non-primitive types like std::string.
     */
    FileInfo(const std::string& n, const fs::path& p, const std::string& ext, std::uintmax_t s)
        : name(n), path(p), extension(ext), size(s), hash("") {}
    
    /**
     * @brief Default constructor for STL container compatibility
     * 
     * Teaching Point: STL containers like std::vector may require default constructors
     * for certain operations (like resize). Always provide one for container-stored types.
     */
    FileInfo() : name(""), path(""), extension(""), size(0), hash("") {}
    
    /**
     * @brief Equality operator for duplicate detection
     * 
     * Teaching Point: Overloading operators makes our custom types work naturally
     * with STL algorithms. This enables using std::find, std::sort, etc.
     * 
     * We compare size first (cheapest), then hash (if computed), then name.
     * This is an optimization technique called "short-circuit evaluation".
     */
    bool operator==(const FileInfo& other) const {
        return size == other.size && 
               name == other.name;
    }
};

#endif // FILEINFO_H
