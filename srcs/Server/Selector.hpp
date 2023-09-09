#pragma once

#include <common_inc.h>
#include <Locations.hpp>
#include <ContentTypeCollection.hpp>
#include <AllowedCGIExecutors.hpp>
#include <ThreadPool.hpp>

const int EVENTS_NUM = 1024;
const int INFINITE = -1;

class Selector {
public:
    explicit Selector(int serverSocket, 
                    int epollSocket,
                    Locations* locations,
                    ContentTypeCollection* contentTypeCollection,
                    AllowedCGIExecutors* allowedCGI,
                    const std::string& serverRoot,
                    const std::string& CGILocation,
                    const std::string& serverName);
    void run(ThreadPool& tp);

private:
    struct epoll_event      m_events[EVENTS_NUM];
    std::string             m_serverRoot;
    std::string             m_CGILocation;
    std::string             m_serverName;
    ContentTypeCollection*  m_contentTypes;
    Locations*              m_locations;
    AllowedCGIExecutors*    m_allowedCGI;
    int                     m_serverSocket;
    int                     m_epollSocket;
};
