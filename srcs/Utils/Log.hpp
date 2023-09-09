#pragma once

#include <common_inc.h>

inline void LogMessage(const std::string& logString) {
    const int MAXLEN = 80;
    char strTime[MAXLEN];
    time_t timeStruct;
    time(&timeStruct);
    struct tm* timeInfo = localtime(&timeStruct);
    strftime(strTime, MAXLEN, "%Y-%m-%dT%X", timeInfo);
    std::cout << "["<< strTime << "] " << logString << std::endl;
}
