#pragma once

#include <common_inc.h>
#include <Locations.hpp>

class Server {
public:
    Server() : ipAddress(""), listenPort(0) { }
    ~Server() { }

    void setListenPort(uint16_t port) {
        listenPort = port;
    }

    void setIpAddress(const std::string& _ipAddress) {
        ipAddress = _ipAddress;
    }

    void addLocation(const std::string& locationName, const std::string& locationRoot) {
        locations.addLocation(locationName, locationRoot);
    }

private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    Locations locations;
    std::string ipAddress;
    uint16_t listenPort;
};