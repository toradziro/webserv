#pragma once

#include <common_inc.h>
#include <ResponseInterface.hpp>

class ResponseGET : public ResponseInterface {
public:
    ResponseGET();
    void sendResponse();

private:
    char*   m_requestLocation;
    int     m_clientFd;
};

class ResponsePOST : public ResponseInterface {
public:
    void sendResponse() { assert(false); }
};

class Response {
public:
    Response(int clientFd,
            std::string ResponseCode,
            std::string contentType,
            int fileFd);
    void sendResponse();

private:
    ResponseType    m_responceType;
    std::string     m_ResponseCode;
    std::string     m_contentType;
    unsigned char*  m_ResponseBody;
    int             m_clientFd;
    int             m_fileSize;
    int             m_fileFd;
};
