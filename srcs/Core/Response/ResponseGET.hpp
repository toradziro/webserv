#pragma once

#include <common_inc.h>
#include <ResponseInterface.hpp>
#include <FileFuncs.hpp>
#include <ContentTypeCollection.hpp>

const int bufferSize = 4096;

class ResponseGET : public ResponseInterface {
public:
    ResponseGET(ResponseInfo info);

    void prepareResponce();
    void sendResponse();

private:
    std::string             m_contentType;
    std::string             m_hostPort;
    std::string             m_serverRoot;
    ContentTypeCollection*  m_contentTypes;
    ResponseCode            m_responseCode;
    FileType                m_fileType;
    char*                   m_requestLocation;
    char*                   m_tmpFilePath;
    int                     m_clientFd;
    int                     m_bodySize;
    int                     m_fileFd;
    char                    m_buffer[bufferSize];

    void fillIndex();
    void fillRegularFileInfo();
    std::string codeToString();
    int getNextChank();
};
