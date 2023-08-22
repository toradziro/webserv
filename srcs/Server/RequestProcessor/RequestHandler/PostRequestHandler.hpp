#pragma once

#include <RequestHandler.hpp>
#include <RequestConfig.hpp>

class PostRequestHandler : public virtual RequestHandler {
public:
    explicit PostRequestHandler(RequestConfig requestConfig) : m_requestConfig(std::move(requestConfig)) { }
    PostRequestHandler(PostRequestHandler&) = delete;
    PostRequestHandler operator=(PostRequestHandler&) = delete;
    ~PostRequestHandler() { }

    void prepareResponce() {}
    void sendResponse() {}
private:

    RequestConfig m_requestConfig;
};
