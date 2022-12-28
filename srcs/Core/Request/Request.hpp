#pragma once

#include <common_inc.h>
#include <Response.hpp>
#include <Locations.hpp>
#include <RequestInterface.hpp>
#include <ContentTypeCollection.hpp>

#ifndef MSG_LEN
# define MSG_LEN 8192
#endif

enum RequestCondition {
    RC_PROCESSED,
    RC_UNFINISHED,
};

class RequestGET : public RequestInterface {
public:
    RequestGET(Locations* locations, ContentTypeCollection* contentType, char* requestBody, int clientFd);
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
    void fillAndProcessResponse();
    std::string parseContent(std::string contetTag);
    void splitBody();
    void deleteExtraPath();

    std::vector<std::string>    m_splitBody;
    ResponseInterface*          m_Response;
    Locations*                  m_locations;
    ContentTypeCollection*      m_contentTypes;
    char*                       m_requestBody;
    char*                       m_requestLocation;
    int                         m_clientFd;
};

class RequestPOST : public RequestInterface {
public:
    RequestPOST(Locations* locations, char* requestBody, int clientFd) :
        m_Response(nullptr),
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
    void fillAndProcessResponse() { assert(false); }

    ResponseInterface*  m_Response;
    Locations*          m_locations;
    char*               m_requestBody;
    int                 m_clientFd;
};
