#pragma once

#include <common_inc.h>

class Responce {
public:
    Responce(int clientFd,
            int contentLength,
            std::string responceCode,
            std::string contentType,
            std::string responceBody);
    void sendResponce();

private:
    std::string m_responceCode;
    std::string m_contentType;
    std::string m_responceBody;
    int         m_clientFd;
    int         m_contentLength;
};