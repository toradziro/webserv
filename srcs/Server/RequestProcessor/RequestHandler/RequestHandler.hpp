#pragma once

class RequestHandler {
public:
    virtual ~RequestHandler() { }
    virtual void prepareResponce() = 0;
    virtual void sendResponse() = 0;
};
