#pragma once

#include <common_inc.h>
#include <Locations.hpp>

class Server {
public:
    Server() : m_ipAddress(""), m_serverName(""), m_listenPort(0) { }
    ~Server() { }

    void setListenPort(uint16_t port);
    void setIpAddress(const std::string& ipAddress);
    void setServerName(const std::string& serverName);
    void addLocation(const std::string& locationName, const std::string& locationRoot);

#ifdef _DEBUG
    void printServer();
#endif

private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    Locations m_locations;
    std::string m_ipAddress;
    std::string m_serverName;
    uint16_t m_listenPort;
};