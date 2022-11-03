#pragma once

#include <common_inc.h>
#include <RequestFabric.hpp>
#include <Request.hpp>
#include <Locations.hpp>
#include <ContentTypeCollection.hpp>

class RequestCollection {
public:
    RequestCollection(Locations* locations, ContentTypeCollection* contentTypes) : 
                    m_locations(locations), m_contentTypes(contentTypes) { }
    void processMessage(int clientFd);

private:
    std::vector<RequestInterface*>  m_unfinishedRequests;
    Locations*                      m_locations;
    ContentTypeCollection*          m_contentTypes;
    RequestFabric                   m_fabric;

    void newConnection(int clientFd);
    void keepHandle(int clientFd, RequestInterface* currRec);
    bool getRequest(int clientFd, RequestInterface** request);
    void deleteFromCollection(int clientFd);
};
