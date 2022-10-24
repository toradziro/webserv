#pragma once

#include <common_inc.h>
#include <Request.hpp>
#include <Locations.hpp>

class RequestCollection {
public:
    RequestCollection(Locations* locations) : m_locations(locations) { }
    void processMessage(int clientFd);

private:
    std::vector<RequestInterface*>  m_unfinishedRequests;
    Locations*                      m_locations;

    void newConnection(int clientFd);
    void keepHandle(int clientFd, RequestInterface* currRec);
    bool getRequest(int clientFd, RequestInterface** request);
    void deleteFromCollection(int clientFd);
};
