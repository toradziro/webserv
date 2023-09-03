#pragma once

#include <common_inc.h>

const std::string Error500 = "500 Internal Server Error";
const std::string Error404 = "404 Not Found";
const std::string Error403 = "403 Forbidden";
const std::string Ok200 = "200 OK";

class RequestHandler {
public:
    virtual ~RequestHandler() { }
    virtual void prepareResponce() = 0;
    virtual void sendResponse() = 0;
};
