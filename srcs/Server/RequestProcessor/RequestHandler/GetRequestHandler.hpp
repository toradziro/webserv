#pragma once

#include <RequestHandler.hpp>
#include <RequestConfig.hpp>

class GetRequestHandler : public virtual RequestHandler {
public:
    explicit GetRequestHandler(RequestConfig requestConfig) : m_requestConfig(std::move(requestConfig)) { }
    GetRequestHandler(GetRequestHandler&) = delete;
    GetRequestHandler operator=(GetRequestHandler&) = delete;
    ~GetRequestHandler() { }

    void prepareResponce() {}
    void sendResponse() {}
private:

    RequestConfig m_requestConfig;
};
