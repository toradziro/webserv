#pragma once

#include <common_inc.h>
#include <dirent.h>

enum FileType {
    FT_FILE,
    FT_DIRECTORY,
    
    FT_NONE
};

inline int getFileSize(int fd) {
    struct stat info;

    if(fstat(fd, &info) == -1)
        return -1;
    return info.st_size;
}


inline bool isFileExists(const std::string& name) {
  struct stat info;   
  return (stat(name.c_str(), &info) == 0); 
}

inline std::string getFileExtention(std::string fileName) {
    size_t pos = 0;
    std::string token;
    std::string delimiter = ".";
    while ((pos = fileName.find(delimiter)) != std::string::npos) {
        token = fileName.substr(0, pos);
        fileName.erase(0, pos + delimiter.length());
    }
    return fileName;
}

inline FileType fileType(int fd) {
    struct stat info;

    if(fstat(fd, &info) == -1)
        return FT_NONE;
    if(S_ISDIR(info.st_mode)) {
        return FT_DIRECTORY;
    } else if(S_ISREG(info.st_mode)) {
        return FT_FILE;
    } else if(S_ISLNK(info.st_mode)) {
        return FT_FILE;
    }
    return FT_NONE;
}

inline bool dirContainsFile(std::string dirName, std::string fileName) {
    DIR* dirPointer = opendir(dirName.c_str());
    // TODO: LOOOOOOG NOT ASSERT
    checkError(dirPointer == nullptr, "dir can't be opened")
    struct dirent* dirInfo = nullptr;
    while((dirInfo = readdir(dirPointer)) != NULL) {
        if(dirInfo->d_name == fileName) {
            return true;
        }
    }
    closedir(dirPointer);
    return false;
}

inline std::vector<std::string> listDirContent(std::string dirName) {
    std::vector<std::string> res;
    DIR* dirPointer = opendir(dirName.c_str());
    // TODO: LOOOOOOG NOT ASSERT
    checkError(dirPointer == nullptr, "dir can't be opened")
    struct dirent* dirInfo = nullptr;
    while((dirInfo = readdir(dirPointer)) != NULL) {
        std::string dirName(dirInfo->d_name);
        res.push_back(dirName);
    }
    closedir(dirPointer);
    return res;
}
