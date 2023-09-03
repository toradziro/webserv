#pragma once

#include <common_inc.h>
#include <Locations.hpp>
#include <ContentTypeCollection.hpp>
#include <AllowedCGIExecutors.hpp>

class Server {
public:
    Server();
    ~Server() {
        close(m_serverSocket);
        close(m_epollFd);
        delete m_contentTypes;
    }

    void setListenPort(uint16_t port);
    void setIpAddress(const std::string& ipAddress);
    void setServerName(const std::string& serverName);
    void setContentTypeCollection(ContentTypeCollection* contentTypes);
    void setServerRoot(const std::string& serverRoot);
    void setCgiDirectory(const std::string& cgiDirectory);
    void addLocation(const std::string& locationName, const std::string& locationRoot);
    void addExecutor(const std::string& extention, const std::string& executor);
    void checkServerInstance();
    void prepareForStart();
    void start();

#ifdef _DEBUG
    void printServer();
#endif

private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    Locations               m_locations;
    ContentTypeCollection*  m_contentTypes;
    AllowedCGIExecutors     m_executors;
    struct epoll_event      m_epollEvent;
    std::string             m_ipAddress;
    std::string             m_serverName;
    std::string             m_serverRoot;
    std::string             m_cgiDirectory;
    int                     m_epollFd;
    int                     m_serverSocket;
    uint16_t                m_listenPort;
    bool                    m_isRunning;
};
