#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <string>
#include <filesystem>
using namespace std;

class FileManager {
public:
    static void listFiles(const string& path = ".");
    static bool makeDir(const string& path);
    static bool removeDir(const string& path);
    static bool copyFile(const string& src, const string& dest);
    static bool moveFile(const string& src, const string& dest);
    static bool deleteFile(const string& path);
    static void printWorkingDirectory();
    static bool changeDirectory(const string& path);
};

#endif