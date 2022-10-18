#pragma once

#include <common_inc.h>
#include <Request.hpp>
#include <Locations.hpp>

class RequestCollection {
public:
    RequestCollection() { }
    void processMessage(int clientFd);

private:
    std::vector<RequestInterface*>  m_unfinishedRequests;
    Locations*                      m_locations;

    void newConnection(int clientFd);
    void keepHandle(int clientFd);
    bool hasUnfinishedRequest(int clientFd);
    RequestInterface* getRequest(int clientFd);
    void deleteFromCollection(int clientFd);
};
