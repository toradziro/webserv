#pragma once

#include <common_inc.h>

const std::string Ok200 = "200 OK";

class RequestHandler {
public:
    virtual ~RequestHandler() { }
    virtual void prepareResponce() = 0;
    virtual void sendResponse() = 0;
};
