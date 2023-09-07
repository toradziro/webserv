#pragma once

#include <common_inc.h>
const std::string httpType = "HTTP/1.1";

const int bufferSize = 8192;

enum class PreparationType {
    PT_File,
    PT_Buffer,
    PT_Unknown,
};

class CommonResponseSender {
public:
    CommonResponseSender(int clientFd, int fileFd, std::string responseCode, 
                        std::string contentType, std::string filename);
    CommonResponseSender(int clientFd, std::string responseBody, 
                        std::string responseCode);
    CommonResponseSender() = delete;
    CommonResponseSender(const CommonResponseSender&) = delete;
    CommonResponseSender operator=(const CommonResponseSender&) = delete;

    void sendResponse();

private:
    int getNextChank();

    std::string     m_responseBody;
    std::string     m_responseCode;
    std::string     m_contentType;
    std::string     m_filename;
    PreparationType m_ResponseFSM;
    int             m_clientFd;
    int             m_fileFd;
    size_t          m_currentBufferIt = 0;
    char            m_buffer[bufferSize];
};
