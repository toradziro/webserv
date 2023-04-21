#pragma once

#include <common_inc.h>
#include <RequestCollection.hpp>
#include <Locations.hpp>

#define EVENTS_NUM  1024
#define INFINITE    -1

class Selector {
public:
    explicit Selector(int serverSocket, 
                    int epollSocket,
                    Locations* locations,
                    ContentTypeCollection* contentTypeCollection,
                    const std::string& serverRoot);
    void run();

private:
    struct epoll_event      m_events[EVENTS_NUM];
    std::string             m_serverRoot;
    ContentTypeCollection*  m_contentTypes;
    Locations*              m_locations;
    RequestCollection       m_requests;
    int                     m_serverSocket;
    int                     m_epollSocket;
};
