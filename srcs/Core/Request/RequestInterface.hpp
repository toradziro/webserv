#pragma once

enum RequestType {
    RT_GET,
    RT_POST,

    RT_UNKNOWN,
};

struct RequestInterface {
    virtual ~RequestInterface() {}

    virtual int handleRequest() = 0;
    virtual int getClientFd() = 0;
    virtual char* getBuffer() = 0;
    virtual void setBuffer(char* newBuff) = 0;

protected:
    virtual void fillAndProcessResponse() = 0;
};
