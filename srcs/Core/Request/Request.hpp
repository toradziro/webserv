#pragma once

#include <common_inc.h>
#include <Responce.hpp>
#include <Locations.hpp>

#ifndef MSG_LEN
# define MSG_LEN 8192
#endif

enum RequestType {
    RT_GET,
    RT_POST,

    RT_UNKNOWN,
};

enum RequestCondition {
    RC_PROCESSED,
    RC_UNFINISHED,
};

enum ResponceNum {
    RN_200,
    RN_404,

    RN_LENGTH,
};

struct RequestInterface {
    virtual ~RequestInterface() {}

    virtual int handleRequest() = 0;
    virtual int getClientFd() = 0;
    virtual char* getBuffer() = 0;
    virtual void setBuffer(char* newBuff) = 0;

protected:
    virtual void fillResponce() = 0;
};

class RequestGET : public RequestInterface {
public:
    RequestGET(Locations* locations, char* requestBody, int clientFd);
    ~RequestGET() { 
        free(m_requestBody);
        if(m_requestLocation != nullptr) {
            free(m_requestLocation);
        }
    }
    int handleRequest();
    int getClientFd() { return m_clientFd; }
    char* getBuffer() { return m_requestBody; }
    void setBuffer(char* newBuff) { m_requestBody = newBuff; }

protected:
    void fillResponce();

    Responce*   m_responce;
    Locations*  m_locations;
    char*       m_requestBody;
    char*       m_requestLocation;
    int         m_clientFd;
};

class RequestPOST : public RequestInterface {
public:
    RequestPOST(Locations* locations, char* requestBody, int clientFd) :
        m_responce(nullptr),
        m_locations(locations),
        m_requestBody(requestBody),
        m_clientFd(clientFd)
    {
    }
    int handleRequest() { assert(false); return 0; }
    int getClientFd() { assert(false); return 0; }
    char* getBuffer() { assert(false); return nullptr; }
    void setBuffer(char* /*newBuff*/) { assert(false); }

protected:
    void fillResponce() { assert(false); }

    Responce*   m_responce;
    Locations*  m_locations;
    char*       m_requestBody;
    int         m_clientFd;
};

RequestInterface* requestFabric(char* requestBody, Locations* locations, int clientFd);
