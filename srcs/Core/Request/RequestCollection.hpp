#pragma once

#include <common_inc.h>
#include <RequestFabric.hpp>
#include <Request.hpp>
#include <Locations.hpp>
#include <ContentTypeCollection.hpp>

class RequestCollection {
public:
    RequestCollection(Locations* locations, ContentTypeCollection* contentTypes, const std::string& serverRoot) : 
                    m_serverRoot(serverRoot), m_locations(locations), m_contentTypes(contentTypes) { }
    void processMessage(int clientFd);

private:
    std::vector<RequestInterface*>  m_unfinishedRequests;
    std::string                     m_serverRoot;
    Locations*                      m_locations;
    ContentTypeCollection*          m_contentTypes;
    RequestFabric                   m_fabric;

    void newConnection(int clientFd);
    void keepHandle(int clientFd, RequestInterface* currRec);
    bool getRequest(int clientFd, RequestInterface** request);
    void deleteFromCollection(int clientFd);
};
