#pragma once

#include <common_inc.h>
#include <dirent.h>

enum FileType {
    FT_FILE,
    FT_DIRECTORY,
    
    FT_NONE
};

[[nodiscard]] inline int getFileSize(const std::filesystem::path& pathToFile) {
    return std::filesystem::file_size(pathToFile);
}


[[nodiscard]] inline bool isFileExists(const std::filesystem::path& pathToFile) {
  return std::filesystem::exists(pathToFile); 
}

[[nodiscard]] inline std::string getFileExtention(const std::filesystem::path& pathToFile) {
    return pathToFile.extension();
}

[[nodiscard]]  inline FileType fileType(const std::filesystem::path& pathToFile) {
    if(std::filesystem::is_directory(pathToFile)) {
        return FT_DIRECTORY;
    } else if(std::filesystem::is_regular_file(pathToFile)) {
        return FT_FILE;
    } else if(std::filesystem::is_symlink(pathToFile)) {
        return FT_FILE;
    }
    return FT_NONE;
}

[[nodiscard]] inline bool dirContainsFile(const std::filesystem::path& pathToFile, std::string fileName) {
    std::filesystem::directory_iterator directoryIterator(pathToFile);
    for(const auto& file : directoryIterator) {
        if(file.path().filename() == fileName) {
            return true;
        }
    }
    return false;
}

inline std::vector<std::filesystem::path> listDirContent(const std::filesystem::path& dirName) {
    std::vector<std::filesystem::path> res;
    std::filesystem::directory_iterator directoryIterator(dirName);
    for(const auto& file : directoryIterator) {
        res.push_back(file.path());
    }
    return res;
}

inline std::filesystem::path GetCurrentDirectory() {
    return std::filesystem::current_path();
}

inline bool validateLocation(std::string serverRoot, std::string requestedLocation) {
    // for case:
    // Requested location is: /home/ehillman/my_projs/webserv/test_files/cgi-index
    // Server root is: /home/ehillman/my_projs/webserv/test_files/cgi 
    if(serverRoot[serverRoot.size() - 1] != '/') {
        serverRoot += '/';
    }
    if(requestedLocation[requestedLocation.size() - 1] != '/') {
        requestedLocation += '/';
    }
    std::cout << "Server root is: " << serverRoot << std::endl;
    std::cout << "Requested location is: " << requestedLocation << std::endl;
    for(size_t i = 0; i < serverRoot.size(); ++i) {
        if(serverRoot[i] != requestedLocation[i]) {
            return false;
        }
    }
    return true;
}