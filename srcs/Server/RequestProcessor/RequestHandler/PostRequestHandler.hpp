#pragma once

#include <RequestHandler.hpp>
#include <RequestConfig.hpp>

class PostRequestHandler : public virtual RequestHandler {
public:
    explicit PostRequestHandler(RequestConfig requestConfig);
    PostRequestHandler(PostRequestHandler&) = delete;
    PostRequestHandler operator=(PostRequestHandler&) = delete;
    ~PostRequestHandler() { }

    void prepareResponce();
    void sendResponse();
private:

    RequestConfig m_requestConfig;
    std::string   m_responseCode;
    std::string   m_responseBody;
    bool          m_404Answered = false;
};
