#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <string>
#include <filesystem>

class FileManager {
public:
    static void listFiles(const std::string& path = ".");
    static bool makeDir(const std::string& path);
    static bool removeDir(const std::string& path);
    static bool copyFile(const std::string& src, const std::string& dest);
    static bool moveFile(const std::string& src, const std::string& dest);
    static bool deleteFile(const std::string& path);
    static void printWorkingDirectory();
    static bool changeDirectory(const std::string& path);
};

#endif