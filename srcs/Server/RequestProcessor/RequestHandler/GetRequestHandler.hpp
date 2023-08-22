#pragma once

#include <RequestHandler.hpp>
#include <RequestConfig.hpp>

const int bufferSize = 8192;

class GetRequestHandler : public virtual RequestHandler {
public:
    explicit GetRequestHandler(RequestConfig requestConfig);
    GetRequestHandler(GetRequestHandler&) = delete;
    GetRequestHandler operator=(GetRequestHandler&) = delete;
    ~GetRequestHandler() { }

    void prepareResponce();
    void sendResponse();

private:
    void fillRegularFileInfo();
    void fillIndex();
    int getNextChank();

    RequestConfig m_requestConfig;
    std::string   m_contentType;
    std::string   m_responseCode;
    std::string   m_generatedAutoindex;
    size_t        m_currentAutoindexIt = 0;
    int           m_fileFd = -1;
    int           m_bodySize = -1;
    bool          m_isAutoindex = false;
    char          m_buffer[bufferSize];
};
