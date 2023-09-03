#pragma once

#include <RequestHandler.hpp>
#include <RequestConfig.hpp>

class GetRequestHandler : public virtual RequestHandler {
public:
    explicit GetRequestHandler(RequestConfig requestConfig);
    GetRequestHandler(GetRequestHandler&) = delete;
    GetRequestHandler operator=(GetRequestHandler&) = delete;
    ~GetRequestHandler() { }

    void prepareResponce();
    void sendResponse();

private:
    void fillIndex();

    RequestConfig m_requestConfig;
    std::string   m_contentType;
    std::string   m_responseCode;
    std::string   m_generatedAutoindex;
    std::string   m_CGIResponceBody;
    size_t        m_currentAutoindexIt = 0;
    int           m_fileFd = -1;
    int           m_bodySize = -1;
    bool          m_isAutoindex = false;
    bool          m_isCGIScenario = false;
};
