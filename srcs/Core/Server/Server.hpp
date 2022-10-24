#pragma once

#include <common_inc.h>
#include <Locations.hpp>

class Server {
public:
    Server();
    ~Server() {
        close(m_serverSocket);
        close(m_epollFd);
    }

    void setListenPort(uint16_t port);
    void setIpAddress(const std::string& ipAddress);
    void setServerName(const std::string& serverName);
    void addLocation(const std::string& locationName, const std::string& locationRoot);
    void checkServerInstance();
    void prepareForStart();
    void start();

#ifdef _DEBUG
    void printServer();
#endif

private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    Locations           m_locations;
    struct epoll_event  m_epollEvent;
    std::string         m_ipAddress;
    std::string         m_serverName;
    int                 m_epollFd;
    int                 m_serverSocket;
    uint16_t            m_listenPort;
    bool                m_isRunning;
};
