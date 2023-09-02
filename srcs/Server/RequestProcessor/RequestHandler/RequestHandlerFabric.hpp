#pragma once

#include <GetRequestHandler.hpp>
#include <PostRequestHandler.hpp>

inline std::shared_ptr<RequestHandler> createRequestHandler(RequestConfig requestConfig) {
    switch(requestConfig.m_reqType) 
    {
        case RequestType::RT_GET:
            return std::make_shared<GetRequestHandler>(std::move(requestConfig));
            break;

        case RequestType::RT_POST:
            return std::make_shared<PostRequestHandler>(std::move(requestConfig));
            break;

        case RequestType::RT_PUT:
        case RequestType::RT_UNKNOWN:
            checkError(true, "request type isn't handeled");
            break;
    }
    return nullptr;
}
