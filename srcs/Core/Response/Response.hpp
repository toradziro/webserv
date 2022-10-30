#pragma once

#include <common_inc.h>

class Response {
public:
    Response(int clientFd,
            std::string ResponseCode,
            std::string contentType,
            int fileFd);
    void sendResponse();

private:
    std::string m_ResponseCode;
    std::string m_contentType;
    unsigned char* m_ResponseBody;
    int         m_clientFd;
    int         m_fileSize;
    int         m_fileFd;
};