#pragma once

#include <common_inc.h>

inline int getFileSize(int fd) {
    struct stat st;

    if(fstat(fd, &st) == -1)
        return -1;
    return st.st_size;
}


inline bool isFileExists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

inline std::string getFileExtention(std::string str) {
    size_t pos = 0;
    std::string token;
    std::string delimiter = ".";
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
    }
    return str;
}
