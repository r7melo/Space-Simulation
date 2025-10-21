#include "Tools.h"
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

std::string Tools::getPath(const std::string& relativePath) {
    std::string fullPath;

#if defined(_WIN32) || defined(_WIN64)
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    fullPath = buffer;
    if (fullPath.back() != '\\' && fullPath.back() != '/')
        fullPath += "\\";
#else
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
        fullPath = buffer;
    else
        fullPath = ".";
    if (fullPath.back() != '/')
        fullPath += "/";
#endif

    return fullPath + relativePath;
}
